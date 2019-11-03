#pragma once

#include <nan.h>

namespace anitomy_js {

using Nan::AsyncWorker;
using Nan::Callback;
using v8::Local;
using v8::Value;

class ParserWorker : public AsyncWorker {
public:
  ParserWorker(Local<Value> input, Callback *callback) : AsyncWorker(callback, "anitomy_js::ParserWorker") {}
  ~ParserWorker() = default;

  void Execute() override;
  void HandleOKCallback() override;
};

} // namespace anitomy_js