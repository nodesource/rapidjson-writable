# rapidjson-writable

API to rapidjson that supports writing data asynchronously. Uses libuv to block the parser thread while waiting for data.

```cpp
#include <iostream>
#include <fstream>
#include <unistd.h>

#define CHUNK_SIZE 64

#include "rapidjson_writable.h"
#include "util.h" // see test/util.h

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

  writable.finish(true);
}

int main(int argc, const char* argv[]) {
  DumpWritable writable;
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
