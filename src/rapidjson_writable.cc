#include "rapidjson_writable.h"

#include <string.h>

//
// write and finish run on Writer (main) thread
//
RapidjsonWritable::RapidjsonWritable() : stream_(work_) {
  RAPIDJSON_WRITABLE_ASSERT(0 == uv_mutex_init(&work_.mutex), "unable to init mutex");
  RAPIDJSON_WRITABLE_ASSERT(0 == uv_cond_init(&work_.cv), "unable to init convar");
  uv_thread_create(&thread_, startParser_, this);
  work_.thread = thread_;
}

void RapidjsonWritable::write(const char& chunk, size_t size) {
  char* buffer = copy_buffer(&chunk, size);
  uv_mutex_lock(&work_.mutex);
  {
    work_.chunks.push_back({
      .buffer = buffer,
      .idx = 0,
      .size = size
    });

    uv_cond_signal(&work_.cv);
  }
  uv_mutex_unlock(&work_.mutex);
}

void RapidjsonWritable::wait() {
  uv_thread_join(&thread_);
}

//
// Parser and cleanup runs on background thread
//
void RapidjsonWritable::startParser_(void* data) {
  RapidjsonWritable* self = static_cast<RapidjsonWritable*>(data);

  self->reader_.IterativeParseInit();

  while (!self->reader_.IterativeParseComplete()) {
    self->reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(self->stream_, self->handler_);
    if (self->reader_.HasParseError()) {
      self->cleanup_();
      self->onparserFailure(self->reader_);
      return;
    } else {
      self->onparsedToken(self->handler_);
    }
  }
  self->cleanup_();
  self->onparseComplete();
}

void RapidjsonWritable::cleanup_() {
  uv_mutex_lock(&work_.mutex);
  {
    for (chunk_t chunk : work_.chunks) {
      delete[] chunk.buffer;
    }
  }
  uv_mutex_unlock(&work_.mutex);
}
