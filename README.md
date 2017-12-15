# rapidjson-writable [![build status](https://secure.travis-ci.org/nodesource/rapidjson-writable.svg?branch=master)](http://travis-ci.org/nodesource/rapidjson-writable)

API to rapidjson that supports writing data asynchronously. Uses libuv to block the parser thread while waiting for data.

```cpp
#include <iostream>
#include <fstream>
#include <unistd.h>

#define CHUNK_SIZE 64

#include "test_assert.h"  // see test/test_assert.h
#include "util.h"         // see test/util.h
#include "rapidjson_writable.h"

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

  // wait for parser thread to finish before exiting
  writable.wait();
}

int main(int argc, const char* argv[]) {
  DumpWritable writable;
  const TestWritableResult* r = writable.init(TestWritableResult::OK());
  if (r->hasError) {
    fprintf(stderr, "Encountered writable init error: %s\n", r->errorMsg);
    delete r;
    return 1;
  }
  delete r;

  const char* file = argv[1];
  std::ifstream ifs(file);
  write_chunks(ifs, writable);
}
```

## Testing Manually

### Dump Tokens

```sh
make clean && make test_dump

./bin/test_dump file.json
# or
cat file.json | ./bin/test_dump
```
