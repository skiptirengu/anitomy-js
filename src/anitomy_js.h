/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#pragma once

#include <anitomy/anitomy.h>
#include <nan.h>
#include <string>
#include <vector>

namespace anitomyJs {

using namespace v8;
using namespace anitomy;
using namespace std;

class AnitomyJs {
public:
  void SetInput(Local<Value> value, Isolate *isolate);
  bool SetOptions(Local<Object> value, Isolate *isolate);
  void Parse();

  vector<Elements> Parsed();
  Local<Value> ParsedResult(Isolate *isolate);

private:
  Anitomy anitomy_;
  vector<wstring> input_;

  vector<Elements> parsed_;
  bool is_batch_parse_;

  wstring ToWideString(Local<Value> str, Isolate *isolate);
  string ToStr(string_t str);

  bool BoolOption(const char *name, Local<Object> value, Isolate *isolate);

  Local<Object> BuildObject(Elements &elements, Isolate *isolate);
  void SetEntry(Local<Object> &object, Isolate *isolate, const char *entry, Elements &elements, ElementCategory pos);
  Local<Array> CategoryArray(Elements &elements, ElementCategory pos, Isolate *isolate);
};
} // namespace anitomyJs