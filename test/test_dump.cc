#include <iostream>
#include <fstream>
#include <unistd.h>

#include "util.h"

#include "rapidjson_writable.h"

#define CHUNK_SIZE 640000

using namespace rapidjson_writable;

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
  assert(!writable.initialized());

  const TestWritableResult* r = writable.init(TestWritableResult::OK());
  if (r->hasError) {
    fprintf(stderr, "Encountered writable init error: %s\n", r->errorMsg);
    delete r;
    return 1;
  }
  delete r;
  assert(writable.initialized());

  if (argc >= 2) {
    const char* file = argv[1];
    std::ifstream ifs(file);
    write_chunks(ifs, writable);
  } else {
    write_chunks(std::cin, writable);
  }
}
