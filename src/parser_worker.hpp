#pragma once

#include "file_parser.hpp"
#include <napi.h>

namespace anitomy_js {

using Napi::Promise;

class ParserWorker : public Napi::AsyncWorker {
public:
  ParserWorker(const Napi::Env &env, const Napi::Value &input, const Napi::Value &options)
      : AsyncWorker(env, "anitomy_js::ParserWorker"), _parser(env, input, options),
        _deferred(Promise::Deferred::New(env)) {
    //
  }

  ~ParserWorker() = default;

  void Execute() override;
  void OnOK() override;
  void OnError(Napi::Error const &error) override;
  Napi::Promise GetPromise();

private:
  FileParser _parser;
  Promise::Deferred _deferred;
};

} // namespace anitomy_js