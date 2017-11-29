#ifndef RAPIDJSON_SAXHANDLER_H
#define RAPIDJSON_SAXHANDLER_H

#include "rapidjson/rapidjson.h"
#include <string>

enum JsonType {
  // Value types
  Null,
  Bool,
  Int,
  Uint,
  Int64,
  Uint64,
  Double,
  Number,
  String,
  
  // Parser Signal types
  Key,
  StartObject,
  EndObject,
  StartArray,
  EndArray
};

struct SaxHandler {
  public:
    JsonType type;
    std::string stringVal;
    bool boolVal; 
    int32_t intVal;
    uint32_t uintVal;
    int64_t int64Val;
    uint64_t uint64Val;
    double doubleVal;

    SaxHandler() : type() {}

    bool Null() {
      type = JsonType::Null;
      stringVal.clear();
      return true;
    }

    bool Bool(bool b) {
      type = JsonType::Bool;
      boolVal = b;
      return true;
    }

    bool Int(int i) {
      type = JsonType::Int;
      intVal = i;
      return true;
    }

    bool Uint(unsigned u) {
      type = JsonType::Uint;
      uintVal = u;
      return true;
    }

    bool Int64(int64_t i) {
      type = JsonType::Int64;
      int64Val = i;
      return true;
    }

    bool Uint64(uint64_t u) {
      type = JsonType::Uint64;
      uint64Val = u;
      return true;
    }

    bool Double(double d) {
      type = JsonType::Double;
      doubleVal = d;
      return true;
    }

    bool RawNumber(const char* str, rapidjson::SizeType length, bool) {
      type = JsonType::Number;
      stringVal = std::string(str, length);
      return true;
    }
    bool String(const char* str, rapidjson::SizeType length, bool) {
      type = JsonType::String;
      stringVal = std::string(str, length);
      return true;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool) {
      type = JsonType::Key;
      stringVal = std::string(str, length);
      return true;
    }

    bool StartObject() {
      type = JsonType::StartObject;
      return true;
    }

    bool EndObject(rapidjson::SizeType memberCount) {
      type = JsonType::EndObject;
      uintVal = memberCount;
      return true;
    }

    bool StartArray() {
      type = JsonType::StartArray;
      return true;
    }

    bool EndArray(rapidjson::SizeType elementCount) {
      type = JsonType::EndArray;
      uintVal = elementCount;
      return true;
    }

  private:
    SaxHandler(const SaxHandler& noCopyConstruction);
    SaxHandler& operator=(const SaxHandler& noAssignment);
};

#endif
