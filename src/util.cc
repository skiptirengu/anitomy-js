#include "util.hpp"

namespace anitomy_js {

using std::function;

void MapNodeArray(Napi::Value value, function<void(Napi::Value)> callback) {
  if (!value.IsArray()) {
    return;
  }
  auto array = value.As<Napi::Array>();
  for (unsigned int i = 0; i < array.Length(); i++) {
    auto current = array.Get(i);
    callback(current);
  }
}

} // namespace anitomy_js