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
  Isolate *isolate = Isolate::GetCurrent();

  Local<Value> argv[] = {anitomy_.ParsedResult(isolate)};
  callback->Call(1, argv, async_resource);
}
} // namespace anitomyJs