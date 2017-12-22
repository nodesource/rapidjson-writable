#ifndef RAPIDJSON_WRITABLE_WRITABLE_H
#define RAPIDJSON_WRITABLE_WRITABLE_H

#include "rapidjson_writable_common.h"
#include <assert.h>
#include "rapidjson_istreamwrapper.h"
#include "rapidjson_saxhandler.h"
#include "rapidjson/reader.h"

#ifndef RAPIDJSON_WRITABLE_ASSERT
#define RAPIDJSON_WRITABLE_ASSERT(cond, s) assert(cond && s)
#endif

namespace rapidjson_writable {

class RapidjsonWritable {
  public:
    RapidjsonWritable() 
      : stream_(work_), initialized_(false) {}

    ~RapidjsonWritable() {
      if (initialized_) {
        uv_mutex_destroy(&work_.mutex);
      }
    }

    //
    // init, write and wait run on Writer (main) thread
    //
    template <typename T>
    const T* init(const T* ok) {
      RAPIDJSON_WRITABLE_ASSERT(0 == uv_mutex_init(&work_.mutex), "unable to init mutex");
      RAPIDJSON_WRITABLE_ASSERT(0 == uv_cond_init(&work_.cv), "unable to init convar");
      uv_thread_create(&thread_, startParser_, this);
      work_.thread = thread_;
      initialized_ = true;
      return ok;
    }

    void write(const char& chunk, size_t size) {
      char* buffer = copy_buffer(&chunk, size);
      uv_mutex_lock(&work_.mutex);
      {
        work_.chunks.push_back({
          .buffer = buffer,
          .size = size,
          .idx = 0
        });

        uv_cond_signal(&work_.cv);
      }
      uv_mutex_unlock(&work_.mutex);
    }

    void wait() {
      uv_thread_join(&thread_);
    }

    bool initialized() { return initialized_; }

  private:
    //
    // Parser and cleanup runs on background thread
    //
    static void startParser_(void* data) {
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

    void cleanup_() {
      uv_mutex_lock(&work_.mutex);
      {
        for (chunk_t chunk : work_.chunks) {
          delete[] chunk.buffer;
        }
      }
      uv_mutex_unlock(&work_.mutex);
    }

  protected:
    //
    // The below methods need to be implemented by the user of the API
    //
    virtual void onparserFailure(rapidjson::Reader& reader) = 0;
    virtual void onparsedToken(SaxHandler& handler) = 0;
    virtual void onparseComplete() = 0;

  private:
    worker_t work_;
    uv_thread_t thread_;

    SaxHandler handler_;
    rapidjson::Reader reader_;
    IStreamWrapper stream_;
    bool initialized_;

    RapidjsonWritable(const RapidjsonWritable& noCopyConstruction);
    RapidjsonWritable& operator=(const RapidjsonWritable& noAssignment);
};

}
#endif
