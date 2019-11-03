#pragma once

#include <anitomy/anitomy.h>
#include <nan.h>

namespace anitomy_js {

using anitomy::ElementCategory;
using anitomy::Elements;
using v8::Array;
using v8::Local;
using v8::Object;
using v8::Value;

class Parser {
public:
  Local<Value> Parse(Local<Value> input, Local<Value> options);

private:
  void AddKey(Local<Object> &object, const char *entry, Elements &elements, ElementCategory pos);
  Local<Array> MultipleElements(Elements &elements, ElementCategory pos);
  Local<Object> BuildObject(Elements &elements);
};

} // namespace anitomy_js