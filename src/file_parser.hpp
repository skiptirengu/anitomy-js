#pragma once

#include <anitomy/anitomy.h>
#include <nan.h>
#include <vector>

namespace anitomy_js {

using anitomy::ElementCategory;
using anitomy::Elements;
using anitomy::Options;
using anitomy::string_t;
using std::vector;
using v8::Array;
using v8::Local;
using v8::Object;
using v8::Value;

class FileParser {
public:
  FileParser(Local<Value> input, Local<Value> options);
  ~FileParser() = default;

  void Parse();
  Local<Value> Result();

private:
  bool _batch;
  vector<string_t> _input;
  vector<Elements> _output;
  Options _options;

  void AddKey(Local<Object> &object, const char *entry, const Elements &elements, ElementCategory pos);
  Local<Object> CoerceArray(Local<Value> value, unsigned int &length);
  Local<Array> MultipleElements(const Elements &elements, ElementCategory pos);
  Local<Object> BuildObject(const Elements &elements);
};

} // namespace anitomy_js