#pragma once

#include <anitomy/anitomy.h>
#include <functional>
#include <napi.h>
#include <vector>

namespace anitomy_js {

using anitomy::Anitomy;
using anitomy::ElementCategory;
using anitomy::Elements;
using anitomy::string_t;
using std::function;
using std::vector;

class FileParser {
public:
  FileParser(Napi::Env env, Napi::Value input, Napi::Value options);
  ~FileParser() = default;

  void Parse();
  Napi::Value Result();

private:
  Anitomy _anitomy;
  Napi::Env _env;

  bool _batch;
  vector<string_t> _input;
  vector<Elements> _output;

  // Javascript object methods
  void AddKey(Napi::Object &object, const char *entry, const Elements &elements, ElementCategory pos);
  Napi::Value CoerceArray(Napi::Value value);
  Napi::Array MultipleElements(const Elements &elements, ElementCategory pos);
  Napi::Object BuildObject(const Elements &elements);

  // Option conversion
  void SafeSetOption(Napi::Value options, const char *key, function<void(Napi::Value)> callback);
  void SetStringVectorOption(Napi::Value options, const char *key, vector<string_t> &val);
  void SetStringOption(Napi::Value options, const char *key, string_t &val);
  void SetBoolOption(Napi::Value options, const char *key, bool &val);
};

} // namespace anitomy_js