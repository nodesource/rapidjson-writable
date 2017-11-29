#ifndef RAPIDJSON_ISTREAMWRAPPER_H
#define RAPIDJSON_ISTREAMWRAPPER_H

#include <assert.h>
#include "common.h"
#include "rapidjson/istreamwrapper.h"

class IStreamWrapper {
  public:
    typedef char Ch;
    IStreamWrapper(worker_t& work) : work_(work), count_() {}

    inline IStreamWrapper::Ch Next(bool take) {
      worker_t& work = work_;
      char c;

      uv_mutex_lock(&work.mutex);
      {
        while (work.chunks.size() == 0) {
          uv_cond_wait(&work.cv, &work.mutex);
        }

        chunk_t& chunk = work.chunks.front();
        c = chunk.buffer[chunk.idx];

        if (take) {
          chunk.idx++;
          if (chunk.idx >= chunk.size) {
            delete[] chunk.buffer;
            work.chunks.erase(work.chunks.begin());
          }
        }

      }
      uv_mutex_unlock(&work.mutex);
      if (take) count_++;

      bool eof = RAPIDJSON_LIKELY(c == '\0');

      // fprintf(stderr, "%s: '%c' %s\n", take ? "take" : "peek",  c, eof ? "EOF" : "");

      return eof
        ? static_cast<Ch>('\0')
        : static_cast<Ch>(c);

    }

    inline IStreamWrapper::Ch Peek() {
      return Next(false);
    }

    inline IStreamWrapper::Ch Take() {
      return Next(true);
    }

    inline size_t Tell() const {
      return count_;
    }

    // ReadStream implemented only for now
    Ch* PutBegin() { assert(false); return 0; }
    void Put(Ch) { assert(false); }
    void Flush() { assert(false); }
    size_t PutEnd(Ch*) { assert(false); return 0; }

  private:
    IStreamWrapper(const IStreamWrapper&);
    IStreamWrapper& operator=(const IStreamWrapper&);
    worker_t& work_;
    size_t count_;
};

#endif
