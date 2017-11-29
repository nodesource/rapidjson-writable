#include "rapidjson_writable.h"

#include <string.h>
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"

static void printParserFailure(rapidjson::Reader& reader) {
  const rapidjson::ParseErrorCode errorCode = reader.GetParseErrorCode();
  const size_t offset = reader.GetErrorOffset();
  fprintf(stderr, "Parser Error at offset %ld: [ %s ]\n", offset , rapidjson::GetParseError_En(errorCode));
}

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

void printHandlerResult(SaxHandler& handler) {
  fprintf(stderr, "Token: %s", jsonTypeToString(handler.type).c_str());
  switch (handler.type) {
    case Bool   : fprintf(stderr, " %s", handler.boolVal ? "true": "false"); break;
    case Int    : fprintf(stderr, " %d", handler.intVal); break;
    case Uint   : fprintf(stderr, " %d", handler.uintVal); break;
    case Int64  : fprintf(stderr, " %lld", handler.int64Val); break;
    case Uint64 : fprintf(stderr, " %lld", handler.uint64Val); break;
    case Double : fprintf(stderr, " %f", handler.doubleVal); break;
    case Number:
    case String:
    case Key:
      fprintf(stderr, " \"%s\"", handler.stringVal.c_str());
      break;
    default:
      break;
  }
  fprintf(stderr, "\n");
}

Writable::Writable() : stream_(work_) {
  assert(0 == uv_mutex_init(&work_.mutex));
  assert(0 == uv_cond_init(&work_.cv));
  uv_thread_create(&thread_, startParser_, this);
  work_.thread = thread_;
}

void Writable::write(const char* chunk, size_t size) {
  char* buffer = copy_buffer(chunk, size);
  uv_mutex_lock(&work_.mutex);
  {
    work_.chunks.push_back({
      .buffer = buffer,
      .idx = 0,
      .size = size
    });

    uv_cond_signal(&work_.cv);
  }
  uv_mutex_unlock(&work_.mutex);
}

void Writable::startParser_(void* data) {
  Writable* self = static_cast<Writable*>(data);

  self->reader_.IterativeParseInit();

  while (!self->reader_.IterativeParseComplete()) {
    self->reader_.IterativeParseNext<rapidjson::kParseDefaultFlags>(self->stream_, self->handler_);
    if (self->reader_.HasParseError()) {
      printParserFailure(self->reader_);
      break;
    } else {
      printHandlerResult(self->handler_);
    }
  }
}
void Writable::finish(bool wait) {
  write("\0", 1);
  if (wait) uv_thread_join(&thread_);
}
