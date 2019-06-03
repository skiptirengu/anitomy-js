/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#pragma once

#include "anitomy_js.h"
#include <nan.h>

namespace anitomyJs {

using namespace v8;

class Worker : public Nan::AsyncWorker {
public:
  Worker(Nan::Callback *callback);
  anitomyJs::AnitomyJs *GetAnitomy();
  void Execute();

protected:
  void HandleOKCallback();

private:
  anitomyJs::AnitomyJs anitomy_;
};
} // namespace anitomyJs