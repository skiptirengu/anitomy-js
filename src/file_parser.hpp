#pragma once

#include <anitomy/anitomy.h>
#include <functional>
#include <nan.h>
#include <vector>

namespace anitomy_js {

using anitomy::Anitomy;
using anitomy::ElementCategory;
using anitomy::Elements;
using anitomy::string_t;
using std::function;
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
  Anitomy _anitomy;

  bool _batch;
  vector<string_t> _input;
  vector<Elements> _output;

  // Javascript object methods
  void AddKey(Local<Object> &object, const char *entry, const Elements &elements, ElementCategory pos);
  Local<Value> CoerceArray(Local<Value> value);
  Local<Array> MultipleElements(const Elements &elements, ElementCategory pos);
  Local<Object> BuildObject(const Elements &elements);

  // Option conversion
  void SafeSetOption(Local<Value> options, const char *key, function<void(Local<Value>)> callback);
  void SetStringVectorOption(Local<Value> options, const char *key, vector<string_t> &val);
  void SetStringOption(Local<Value> options, const char *key, string_t &val);
  void SetBoolOption(Local<Value> options, const char *key, bool &val);
};

} // namespace anitomy_js