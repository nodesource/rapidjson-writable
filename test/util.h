#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "rapidjson/error/en.h"

// Need to include this before the writable to be able to override
// RAPIDJSON_WRITABLE_ASSERT
#include "test_assert.h"
#include "rapidjson_writable.h"

using namespace rapidjson_writable;

std::string jsonTypeToString(JsonType type) {
  switch(type) {
    // Value types
    case Null: return "null";
    case Bool: return "bool";
    case Int: return "int";
    case Uint: return "uint";
    case Int64: return "int64";
    case Uint64: return "uint64";
    case Double: return "double";
    case Number: return "number";
    case String: return "string";

    // Parser Signal types
    case Key: return "[key]";
    case StartObject: return "[start object]";
    case EndObject: return "[end object]";
    case StartArray: return "[start array]";
    case EndArray: return "[end array]";
  }
}

void printParsedToken(SaxHandler& handler) {
  fprintf(stdout, "Token: %s", jsonTypeToString(handler.type).c_str());
  switch (handler.type) {
    case Bool   : fprintf(stdout, " %s", handler.boolVal ? "true": "false"); break;
    case Int    : fprintf(stdout, " %d", handler.intVal); break;
    case Uint   : fprintf(stdout, " %d", handler.uintVal); break;
    case Int64  : fprintf(stdout, " %lld", handler.int64Val); break;
    case Uint64 : fprintf(stdout, " %llu", handler.uint64Val); break;
    case Double : fprintf(stdout, " %f", handler.doubleVal); break;
    case Number:
    case String:
    case Key:
      fprintf(stdout, " \"%s\"", handler.stringVal.c_str());
      break;
    default:
      break;
  }
  fprintf(stdout, "\n");
}

void printParserFailure(rapidjson::Reader& reader) {
  const rapidjson::ParseErrorCode errorCode = reader.GetParseErrorCode();
  const size_t offset = reader.GetErrorOffset();
  fprintf(stderr, "Parser Error at offset %ld: [ %s ]\n", offset , rapidjson::GetParseError_En(errorCode));
}

void echoParsedToken(SaxHandler& handler, bool addComma) {
  if (addComma) fprintf(stdout, ",");
  switch (handler.type) {
    case Bool   : fprintf(stdout, "%s", handler.boolVal ? "true": "false"); break;
    case Int    : fprintf(stdout, "%d", handler.intVal); break;
    case Uint   : fprintf(stdout, "%d", handler.uintVal); break;
    case Int64  : fprintf(stdout, "%lld", handler.int64Val); break;
    case Uint64 : fprintf(stdout, "%llu", handler.uint64Val); break;
    case Double : fprintf(stdout, "%f", handler.doubleVal); break;
    case Number : fprintf(stdout, "%s", handler.stringVal.c_str()); break;
    case String : fprintf(stdout, "\"%s\"", handler.stringVal.c_str()); break;
    case Null   : fprintf(stdout, "null"); break;

    // Parser Signal types
    case Key         : fprintf(stdout, "\"%s\": ", handler.stringVal.c_str()); break;
    case StartObject : fprintf(stdout, "{"); break;
    case EndObject   : fprintf(stdout, "}"); break;
    case StartArray  : fprintf(stdout, "["); break;
    case EndArray    : fprintf(stdout, "]"); break;
  }
  fflush(stdout);
}

#endif
