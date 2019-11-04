#pragma once

#include <codecvt>
#include <functional>
#include <locale>
#include <nan.h>
#include <string>

namespace anitomy_js {

typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_cvt;

inline std::string wstring_to_string(const std::wstring &input) {
  static wstring_cvt conv;
  return conv.to_bytes(input);
}

inline std::wstring string_to_wstring(const std::string &input) {
  static wstring_cvt conv;
  return conv.from_bytes(input);
}

inline v8::Local<v8::String> wstring_to_node_string(const std::wstring &str) {
  return Nan::New(wstring_to_string(str)).ToLocalChecked();
}

inline std::wstring node_string_to_wstring(const v8::Local<v8::Value> value) {
  Nan::Utf8String str(value);
  return str.length() > 0 ? string_to_wstring(*str) : L"";
}

inline v8::Local<v8::String> node_string(const char *str) {
  return Nan::New<v8::String>(str).ToLocalChecked();
}

void MapNodeArray(v8::Local<v8::Value> value, std::function<void(v8::Local<v8::Value>)> callback);

} // namespace anitomy_js
