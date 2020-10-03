#include "file_parser.hpp"
#include "parser_worker.hpp"
#include "util.hpp"
#include "validate.hpp"
#include <napi.h>

namespace anitomy_js {

using Napi::Promise;

Napi::Value ParseAsync(const Napi::CallbackInfo &info) {
  auto env = info.Env();

  Napi::Value input;
  Napi::Object options;

  auto error = ParseAndValidate(info, input, options);

  if (error) {
    auto deferred = Promise::Deferred::New(env);
    auto node_error = Napi::Error::New(env, error);
    deferred.Reject(node_error.Value());
    return deferred.Promise();
  }

  auto worker = new ParserWorker(env, input, options);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value ParseSync(const Napi::CallbackInfo &info) {
  auto env = info.Env();

  Napi::Value input;
  Napi::Object options;

  auto error = ParseAndValidate(info, input, options);

  if (error) {
    Napi::TypeError::New(env, error).ThrowAsJavaScriptException();
    return env.Undefined();
  }

  auto parser = new FileParser(env, input, options);
  parser->Parse();

  return parser->Result();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(node_string(env, "parseSync"), Napi::Function::New(env, ParseSync));
  exports.Set(node_string(env, "parseAsync"), Napi::Function::New(env, ParseAsync));
  return exports;
}

NODE_API_MODULE(anitomy, Init);

} // namespace anitomy_js