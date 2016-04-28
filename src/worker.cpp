/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "worker.h"

namespace anitomyJs {
    
    Worker::Worker(Nan::Callback* callback) : Nan::AsyncWorker(callback) { };
    
    void Worker::Execute() {
        anitomy_.Parse();
    }
    
    anitomyJs::AnitomyJs* Worker::GetAnitomy() {
        return &anitomy_;
    }
    
    void Worker::HandleOKCallback() {
        Nan::HandleScope scope;
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        
        v8::Local<v8::Value> argv[] = { anitomy_.ParsedResult(isolate) };
        callback->Call(1, argv);
    }
}