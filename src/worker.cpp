/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#include "worker.h"

namespace anitomyJs {

Worker::Worker(Nan::Callback *callback) : Nan::AsyncWorker(callback, "anitomyJs:Worker"){};

void Worker::Execute() { anitomy_.Parse(); }

anitomyJs::AnitomyJs *Worker::GetAnitomy() { return &anitomy_; }

void Worker::HandleOKCallback() {
  Nan::HandleScope scope;
  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  v8::Local<v8::Value> argv[] = {anitomy_.ParsedResult(isolate)};
  callback->Call(1, argv, async_resource);
}
}