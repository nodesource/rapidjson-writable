#ifndef TEST_TEST_ASSERT_H
#define TEST_TEST_ASSERT_H

#include <string.h>

inline char* scopy(const char* s) {
  size_t len = strlen(s);
  char* cpy = new char[len + 1]();
  strncpy(cpy, s, len + 1);
  return cpy;
}

class TestWritableResult {
  public:
    TestWritableResult()
      : hasError(false), errorMsg(nullptr) {}

    TestWritableResult(bool hasError, const char* msg)
      : hasError(hasError), errorMsg(scopy(msg)) {}

    ~TestWritableResult() {
      if (errorMsg != nullptr) delete[] errorMsg;
    }

    bool hasError;
    const char* errorMsg;

    static TestWritableResult* OK() { return new TestWritableResult(); }

  private:
    TestWritableResult(const TestWritableResult& noCopyConstruction);
    TestWritableResult& operator=(const TestWritableResult& noAssignment);
};

#ifndef RAPIDJSON_WRITABLE_ASSERT
#define TEST_ASSERT(cond, s)                                   \
  do {                                                         \
    if (!(cond)) return new TestWritableResult(true, s); \
  } while(0);

#define RAPIDJSON_WRITABLE_ASSERT TEST_ASSERT
#endif

#endif
