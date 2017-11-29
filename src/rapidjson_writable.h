#ifndef RAPIDJSON_WRITABLE_H
#define RAPIDJSON_WRITABLE_H

#include "common.h"
#include <assert.h>
#include "rapidjson_istreamwrapper.h"
#include "rapidjson_saxhandler.h"
#include "rapidjson/reader.h"

class Writable {
  public:
    Writable();
    void write(const char* chunk, size_t size);
    void finish(bool wait);

  private:
    worker_t work_;
    uv_thread_t thread_;

    SaxHandler handler_; 
    rapidjson::Reader reader_;
    IStreamWrapper stream_;

    static void startParser_(void* data);
};
#endif
