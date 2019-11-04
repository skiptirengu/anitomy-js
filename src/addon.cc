#include "file_parser.hpp"
#include "parser_worker.hpp"
#include "util.hpp"
#include "validate.hpp"
#include <nan.h>

namespace anitomy_js {

using Nan::AsyncQueueWorker;
using Nan::Callback;
using Nan::FunctionCallbackInfo;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;
using Nan::ThrowTypeError;
using Nan::To;
using Nan::Undefined;
using v8::Function;
using v8::FunctionTemplate;
using v8::Local;
using v8::Value;

void ParseAsync(const FunctionCallbackInfo<Value> &args) {
  Local<Value> input, options, callback;
  auto error = ValidateData(args, input, options, callback);

  if (error) {
    ThrowTypeError(error);
    return;
  }

  auto *cb = new Callback(To<Function>(callback).ToLocalChecked());
  AsyncQueueWorker(new ParserWorker(input, options, cb));

  args.GetReturnValue().Set(Undefined());
}

void ParseSync(const FunctionCallbackInfo<Value> &args) {
  Local<Value> input, options;
  auto error = ValidateData(args, input, options);

  if (error) {
    ThrowTypeError(error);
    return;
  }

  auto parser = new FileParser(input, options);
  parser->Parse();
  args.GetReturnValue().Set(parser->Result());
}

NAN_MODULE_INIT(InitAddon) {
  Set(target, node_string("parseSync"), GetFunction(New<FunctionTemplate>(ParseSync)).ToLocalChecked());
  Set(target, node_string("parseAsync"),
      GetFunction(New<FunctionTemplate>(ParseAsync)).ToLocalChecked());
}

NODE_MODULE(anitomy, InitAddon)

} // namespace anitomy_js