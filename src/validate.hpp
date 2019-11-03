#pragma once

#include <nan.h>

namespace anitomy_js {

using Nan::FunctionCallbackInfo;
using v8::Local;
using v8::Value;

const char *ValidateData(const FunctionCallbackInfo<Value> &args, Local<Value> &input, Local<Value> &options);
const char *ValidateData(const FunctionCallbackInfo<Value> &args, Local<Value> &input, Local<Value> &options,
                         Local<Value> &callback);

const char *ValidateCallback(Local<Value> callback);
const char *ValidateInput(Local<Value> input);
const char *ValidateOptions(Local<Value> value);

} // namespace anitomy_js