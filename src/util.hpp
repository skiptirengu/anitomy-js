#pragma once

#include <codecvt>
#include <nan.h>
#include <string>

namespace {

using Nan::New;
using Nan::Utf8String;
using std::codecvt_utf8;
using std::string;
using std::wstring;
using std::wstring_convert;
using v8::Local;
using v8::String;
using v8::Value;

typedef wstring_convert<codecvt_utf8<wchar_t>, wchar_t> wstring_cvt;

inline string wstring_to_string(const wstring &input) {
  static wstring_cvt conv;
  return conv.to_bytes(input);
}

inline wstring string_to_wstring(const string &input) {
  static wstring_cvt conv;
  return conv.from_bytes(input);
}

inline Local<String> wstring_to_node_string(const wstring &str) { return New(wstring_to_string(str)).ToLocalChecked(); }

inline wstring node_string_to_wstring(const Local<Value> value) {
  Utf8String str(value);
  return str.length() > 0 ? string_to_wstring(*str) : L"";
}

} // namespace