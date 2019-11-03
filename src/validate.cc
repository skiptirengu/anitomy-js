#include "validate.hpp"

namespace anitomy_js {

using Nan::FunctionCallbackInfo;
using Nan::Get;
using Nan::Has;
using Nan::New;
using Nan::To;
using Nan::TypeError;
using Nan::Undefined;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

const char *ValidateData(const FunctionCallbackInfo<Value> &args, bool async) {
  auto length = args.Length();

  if (length < 1) {
    return "Input must be either a string or array";
  }

  auto input_error = ValidateInput(args[0]);
  if (input_error) {
    return input_error;
  }

  Local<Value> callback;
  Local<Value> options;

  if (async && length == 2) {
    callback = args[1];
    options = Undefined();
  } else if (async && length >= 3) {
    callback = args[2];
    options = args[1];
  } else {
    callback = Undefined();
    options = args[1];
  }

  auto options_error = ValidateOptions(options);
  if (options_error) {
    return options_error;
  }

  auto callback_error = ValidateCallback(callback);
  if (async && callback_error) {
    return callback_error;
  }

  return NULL;
}

const char *ValidateCallback(Local<Value> callback) {
  if (callback->IsFunction()) {
    return "Callback must be a function";
  } else {
    return NULL;
  }
}

const char *ValidateInput(Local<Value> input) {
  if (!input->IsString() || !input->IsArray()) {
    return "Input must be either a string or array";
  } else {
    return NULL;
  }
}

const char *ValidateOptions(Local<Value> value) {
  if (value->IsNullOrUndefined()) {
    return NULL;
  }

  if (!value->IsObject()) {
    return "Options must be an object";
  }

  Local<Object> options = To<Object>(value).ToLocalChecked();
  Local<String> delimiters = New<String>("allowed_delimiters").ToLocalChecked();
  Local<String> ignored = New<String>("ignored_strings").ToLocalChecked();

  if (Has(options, delimiters).FromJust() && !Get(options, delimiters).ToLocalChecked()->IsString()) {
    return "Option \"allowed_delimiters\" must be a string";
  }

  if (Has(options, ignored).FromJust() && !Get(options, ignored).ToLocalChecked()->IsArray()) {
    return "Option \"ignored_strings\" must be an array";
  }

  return NULL;
}

} // namespace anitomy_js