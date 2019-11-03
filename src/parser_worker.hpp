#pragma once

#include "file_parser.hpp"
#include <nan.h>

namespace anitomy_js {

using Nan::AsyncWorker;
using Nan::Callback;
using Nan::New;
using v8::Local;
using v8::String;
using v8::Value;

class ParserWorker : public AsyncWorker {
public:
  ParserWorker(const Local<Value> input, const Local<Value> options, Callback *callback)
      : AsyncWorker(callback, "anitomy_js::ParserWorker"), _parser(input, options) {}
  ~ParserWorker() = default;

  void Execute() override;
  void HandleOKCallback() override;

private:
  FileParser _parser;
};

} // namespace anitomy_js