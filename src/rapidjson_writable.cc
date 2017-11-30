#include "rapidjson_writable.h"

#include <string.h>

//
// write and finish run on Writer (main) thread
//
RapidjsonWritable::RapidjsonWritable() : stream_(work_) {
  assert(0 == uv_mutex_init(&work_.mutex));
  assert(0 == uv_cond_init(&work_.cv));
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

void RapidjsonWritable::finish(bool wait) {
  const char eof = '\0';
  write(eof, 1);
  if (wait) uv_thread_join(&thread_);
}

//
// Parser runs on background thread
//
void RapidjsonWritable::startParser_(void* data) {
  RapidjsonWritable* self = static_cast<RapidjsonWritable*>(data);

  self->reader_.IterativeParseInit();

  while (!self->reader_.IterativeParseComplete()) {
    self->reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(self->stream_, self->handler_);
    if (self->reader_.HasParseError()) {
      self->onparserFailure(self->reader_);
      return;
    } else {
      self->onparsedToken(self->handler_);
    }
  }
  self->onparseComplete();
}
