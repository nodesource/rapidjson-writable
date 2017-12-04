#include <iostream>
#include <fstream>
#include <unistd.h>

#include "../src/rapidjson_writable.h"
#include "util.h"

#define CHUNK_SIZE 640000

class DumpWritable : public RapidjsonWritable {
  void onparserFailure(rapidjson::Reader& reader) {
    printParserFailure(reader);
  }

  void onparsedToken(SaxHandler& handler) {
    printParsedToken(handler);
  }

  void onparseComplete() {
    fprintf(stdout, "OK\n");
  }
};

void write_chunks(std::istream& stream, RapidjsonWritable& writable) {
  do {
    std::vector<char> buffer (CHUNK_SIZE, 0);
    stream.read(buffer.data(), buffer.size());
    writable.write(*buffer.data(), buffer.size());
    usleep(1E5);
  } while(!stream.eof());

  writable.wait();
}

int main(int argc, const char* argv[]) {
  DumpWritable writable;
  writable.init(nullptr);
  if (argc >= 2) {
    const char* file = argv[1];
    std::ifstream ifs(file);
    write_chunks(ifs, writable);
  } else {
    write_chunks(std::cin, writable);
  }
}
