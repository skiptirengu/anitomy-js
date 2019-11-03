#include "validate.hpp"
#include <nan.h>

namespace anitomy_js {

using Nan::FunctionCallbackInfo;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;
using Nan::ThrowError;
using Nan::Undefined;
using v8::FunctionTemplate;
using v8::Local;
using v8::String;
using v8::Value;

void ParseAsync(const FunctionCallbackInfo<Value> &args) {
  auto error = ValidateData(args, true);

  if (error) {
    ThrowError(error);
  }

  args.GetReturnValue().Set(Undefined());
}

void ParseSync(const FunctionCallbackInfo<Value> &args) {
  auto error = ValidateData(args, false);

  if (error) {
    ThrowError(error);
  }

  args.GetReturnValue().Set(Undefined());
}

NAN_MODULE_INIT(InitAddon) {
  Set(target, New<String>("parseSync").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ParseSync)).ToLocalChecked());
  Set(target, New<String>("parseAsync").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ParseAsync)).ToLocalChecked());
}

NODE_MODULE(anitomy, InitAddon)

} // namespace anitomy_js