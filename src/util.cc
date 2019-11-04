#include "util.hpp"

namespace anitomy_js {

using Nan::Get;
using std::function;
using v8::Array;
using v8::Local;
using v8::Value;

void MapNodeArray(Local<Value> value, function<void(Local<Value>)> callback) {
  if (!value->IsArray()) {
    return;
  }
  auto array = Local<Array>::Cast(value);
  for (unsigned int i = 0; i < array->Length(); i++) {
    auto current = Get(array, i).ToLocalChecked();
    callback(current);
  }
}

} // namespace anitomy_js