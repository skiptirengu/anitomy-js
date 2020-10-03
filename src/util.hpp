#pragma once

#include <codecvt>
#include <functional>
#include <locale>
#include <napi.h>
#include <string>

namespace anitomy_js {

typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_cvt;

inline Napi::String node_string(const Napi::Env env, const char *str) {
  return Napi::String::New(env, str);
}

inline std::string wstring_to_string(const std::wstring &input) {
  static wstring_cvt conv;
  return conv.to_bytes(input);
}

inline std::wstring string_to_wstring(const std::string &input) {
  static wstring_cvt conv;
  return conv.from_bytes(input);
}

inline Napi::String wstring_to_node_string(const Napi::Env env, const std::wstring &str) {
  return node_string(env, wstring_to_string(str).c_str());
}

inline std::wstring node_string_to_wstring(const Napi::Value value) {
  if (value.IsString()) {
    auto str = value.ToString().Utf8Value();
    return str.size() > 0 ? string_to_wstring(str) : L"";
  }
  return L"";
}

void MapNodeArray(Napi::Value value, std::function<void(Napi::Value)> callback);

} // namespace anitomy_js
