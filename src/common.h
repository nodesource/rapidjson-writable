#ifndef COMMON_H
#define COMMON_H

#include <uv.h>
#include <vector>

typedef struct {
  char* buffer;
  size_t size;
  size_t idx;
} chunk_t;

typedef struct {
  uv_cond_t cv;
  uv_mutex_t mutex;
  uv_thread_t thread;
  std::vector<chunk_t> chunks;
} worker_t;

inline char* copy_buffer(const char* buffer, size_t size) {
  char* cpy = new char[size];
  strncpy(cpy, buffer, size);
  return cpy;
}
#endif
