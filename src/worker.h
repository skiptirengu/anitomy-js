/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#ifndef ANITOMY_JS_WORKER_H_
#define ANITOMY_JS_WORKER_H_

#include "anitomy_js.h"
#include <nan.h>

namespace anitomyJs {

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
}

#endif