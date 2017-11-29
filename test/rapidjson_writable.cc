#include <fstream>
#include <vector>
#include <unistd.h>
#include <uv.h>
#include "rapidjson_writable.h"

#include <unistd.h>

#define CHUNK_SIZE 64

void write_chunks(const char* file, Writable& writable) {
  std::ifstream ifs(file);

  do {
    std::vector<char> buffer (CHUNK_SIZE, 0);
    ifs.read(buffer.data(), buffer.size());
    writable.write(buffer.data(), buffer.size());
    usleep(1E5);
  } while(!ifs.eof());

  writable.finish(true);
}

int main(int argc, const char* argv[]) {
  const char* file = argv[1];

  Writable writable;
  write_chunks(file, writable);
}
