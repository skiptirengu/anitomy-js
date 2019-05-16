/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#include "worker.h"
#include <nan.h>

namespace anitomyJs {

using namespace v8;

bool ValidateInput(Local<Value> value, Isolate *isolate) {
  bool valid = value->IsString() || value->IsArray();
  if (!valid) {
    isolate->ThrowException(
        Exception::TypeError(String::NewFromUtf8(isolate, "Wrong data type", NewStringType::kNormal).ToLocalChecked()));
  }
  return valid;
}

bool ValidateOptions(Local<Value> options, Isolate *isolate) {
  if (!options->IsObject()) {
    isolate->ThrowException(
        Exception::TypeError(String::NewFromUtf8(isolate, "Options must be an object", NewStringType::kNormal).ToLocalChecked()));
    return false;
  }
  return true;
}

void ParseSync(const Nan::FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();
  int args_length = args.Length();

  if (args_length < 1) {
    isolate->ThrowException(
        Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments", NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  Local<Value> input = args[0];
  if (!ValidateInput(input, isolate)) {
    return;
  }

  anitomyJs::AnitomyJs anitomy;
  if (args_length >= 2) {
    Local<Value> options = args[1];
    if (!ValidateOptions(options, isolate) ||
        !anitomy.SetOptions(options->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), isolate)) {
      return;
    }
  }

  anitomy.SetInput(input, isolate);
  anitomy.Parse();

  args.GetReturnValue().Set(anitomy.ParsedResult(isolate));
}

void ParseAsync(const Nan::FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();
  int args_length = args.Length();

  if (args_length < 2) {
    isolate->ThrowException(
        Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments", NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  Local<Value> input = args[0];
  if (!ValidateInput(input, isolate)) {
    return;
  }
  if (!args[1]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Second parameter must be a callback", NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  Nan::Callback *callback = new Nan::Callback(args[1].As<Function>());
  anitomyJs::Worker *worker = new anitomyJs::Worker(callback);

  if (args_length >= 3) {
    Local<Value> options = args[2];
    if (!ValidateOptions(options, isolate) ||
        !worker->GetAnitomy()->SetOptions(options->ToObject(isolate->GetCurrentContext()).ToLocalChecked(), isolate)) {
      return;
    }
  }

  worker->GetAnitomy()->SetInput(input, isolate);
  Nan::AsyncQueueWorker(worker);
  args.GetReturnValue().SetUndefined();
}

void Init(Local<Object> exports, Local<Object> module) {
  Isolate *isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  exports->Set(context, Nan::New("parseSync").ToLocalChecked(),
               Nan::New<FunctionTemplate>(ParseSync)->GetFunction(context).ToLocalChecked());
  exports->Set(context, Nan::New("parseAsync").ToLocalChecked(),
               Nan::New<FunctionTemplate>(ParseAsync)->GetFunction(context).ToLocalChecked());
}

NODE_MODULE(anitomy, Init)
} // namespace anitomyJs
