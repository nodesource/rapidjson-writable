#include <iostream>
#include <fstream>
#include <unistd.h>

#include "rapidjson_writable.h"
#include "util.h"

using namespace rapidjson_writable;

class MinifyWritable : public RapidjsonWritable {
  public:
    MinifyWritable() : nextKeyNeedsComma_(), insideArray_(), passedFirstArrayElement_() {}

  private:
    void onparserFailure(rapidjson::Reader& reader) {
      printParserFailure(reader);
    }

    void onparsedToken(SaxHandler& handler) {
      // supporting flat arrays only for testing purposes
      const bool addComma = (
          handler.type == JsonType::Key &&
          nextKeyNeedsComma_
      ) || (
        handler.type != JsonType::EndArray &&
        insideArray_ && passedFirstArrayElement_
      );

      echoParsedToken(handler, addComma);

      nextKeyNeedsComma_ = (
        handler.type  != JsonType::StartObject &&
        handler.type  != JsonType::StartArray
      );

      if (insideArray_) passedFirstArrayElement_ = true;
      bool arrayStarting = handler.type == JsonType::StartArray;
      if (arrayStarting) passedFirstArrayElement_ = false;

      insideArray_ = (insideArray_ && handler.type != JsonType::EndArray) ||
        arrayStarting;
    }

    void onparseComplete() {
      fprintf(stderr, "OK");
    }

    bool nextKeyNeedsComma_;
    bool insideArray_;
    bool passedFirstArrayElement_;
};

void write_chunks(std::istream& stream, RapidjsonWritable& writable, size_t chunkSize) {
  do {
    std::vector<char> buffer (chunkSize, 0);
    stream.read(buffer.data(), buffer.size());
    writable.write(*buffer.data(), buffer.size());
    usleep(1E5);
  } while(!stream.eof());

  writable.wait();
}

int main(int argc, const char* argv[]) {
  MinifyWritable writable;

  assert(!writable.initialized());
  const TestWritableResult* r = writable.init(TestWritableResult::OK());
  if (r->hasError) {
    fprintf(stderr, "Encountered writable init error: %s\n", r->errorMsg);
    delete r;
    return 1;
  }
  delete r;
  assert(writable.initialized());

  const size_t chunkSize = atoi(argv[1]);
  if (argc >= 3) {
    const char* file = argv[2];
    std::ifstream ifs(file);
    write_chunks(ifs, writable, chunkSize);
  } else {
    write_chunks(std::cin, writable, chunkSize);
  }
}
