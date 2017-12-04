#ifndef RAPIDJSON_WRITABLE_WRITABLE_H
#define RAPIDJSON_WRITABLE_WRITABLE_H

#include "common.h"
#include <assert.h>
#include "rapidjson_istreamwrapper.h"
#include "rapidjson_saxhandler.h"
#include "rapidjson/reader.h"

#ifndef RAPIDJSON_WRITABLE_ASSERT
#define RAPIDJSON_WRITABLE_ASSERT(cond, s) assert(cond && s)
#endif

class RapidjsonWritable {
  public:
    RapidjsonWritable();
    void* init(void* ok);
    void write(const char& chunk, size_t size);
    void wait();

  protected:
    virtual void onparserFailure(rapidjson::Reader& reader) = 0;
    virtual void onparsedToken(SaxHandler& handler) = 0;
    virtual void onparseComplete() = 0;

  private:
    worker_t work_;
    uv_thread_t thread_;

    SaxHandler handler_;
    rapidjson::Reader reader_;
    IStreamWrapper stream_;

    void cleanup_();
    static void startParser_(void* data);
};
#endif
