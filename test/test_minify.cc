#include <iostream>
#include <fstream>
#include <unistd.h>

#include "../src/rapidjson_writable.h"
#include "util.h"

#define CHUNK_SIZE 640

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
      fprintf(stderr, "OK\n");
    }

    bool nextKeyNeedsComma_;
    bool insideArray_;
    bool passedFirstArrayElement_;
};

void write_chunks(std::istream& stream, RapidjsonWritable& writable) {
  do {
    std::vector<char> buffer (CHUNK_SIZE, 0);
    stream.read(buffer.data(), buffer.size());
    writable.write(*buffer.data(), buffer.size());
    usleep(1E5);
  } while(!stream.eof());

  writable.finish(true);
}

int main(int argc, const char* argv[]) {
  MinifyWritable writable;
  if (argc >= 2) {
    const char* file = argv[1];
    std::ifstream ifs(file);
    write_chunks(ifs, writable);
  } else {
    write_chunks(std::cin, writable);
  }
}
