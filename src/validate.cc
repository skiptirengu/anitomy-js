#include "validate.hpp"
#include "util.hpp"

namespace anitomy_js {

using Nan::FunctionCallbackInfo;
using Nan::Get;
using Nan::Has;
using Nan::Set;
using Nan::To;
using Nan::Undefined;
using v8::Local;
using v8::Object;
using v8::Value;

const char *ValidateData(const FunctionCallbackInfo<Value> &args, Local<Value> &input,
                         Local<Value> &options) {
  Local<Value> callback = Undefined();
  return ValidateData(args, input, options, callback);
}

const char *ValidateData(const FunctionCallbackInfo<Value> &args, Local<Value> &input,
                         Local<Value> &options, Local<Value> &callback) {
  auto length = args.Length();

  if (length < 1) {
    return "Input must be either a string or array";
  } else {
    input = args[0];
  }

  auto input_error = ValidateInput(args[0]);
  if (input_error) {
    return input_error;
  }

  bool async = callback.IsEmpty();

  if (async && length == 2) {
    callback = args[1];
    options = Undefined();
  } else if (async && length >= 3) {
    callback = args[2];
    options = args[1];
  } else if (length == 1) {
    callback = Undefined();
    options = Undefined();
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
  if (!callback->IsFunction()) {
    return "Callback must be a function";
  } else {
    return NULL;
  }
}

const char *ValidateInput(Local<Value> input) {
  if (!input->IsString() && !input->IsArray()) {
    return "Input must be either a string or array";
  } else {
    return NULL;
  }
}

const char *ValidateOptions(Local<Value> &value) {
  if (value->IsNullOrUndefined()) {
    return NULL;
  }

  if (!value->IsObject()) {
    value = Undefined();
    return NULL;
  }

  auto options = To<Object>(value).ToLocalChecked();
  auto delimiters = node_string("allowed_delimiters");
  auto ignored = node_string("ignored_strings");

  if (Has(options, delimiters).FromJust() && !Get(options, delimiters).ToLocalChecked()->IsString()) {
    Set(options, delimiters, Undefined());
  }

  if (Has(options, ignored).FromJust() && !Get(options, ignored).ToLocalChecked()->IsArray()) {
    Set(options, ignored, Undefined());
  }

  return NULL;
}

} // namespace anitomy_js