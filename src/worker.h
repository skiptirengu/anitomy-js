/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ANITOMY_JS_WORKER_H_
#define ANITOMY_JS_WORKER_H_

#include "anitomy_js.h"
#include <nan.h>

namespace anitomyJs {
    
    class Worker : public Nan::AsyncWorker {
        public:
            Worker(Nan::Callback* callback);
            anitomyJs::AnitomyJs* GetAnitomy();
            void Execute();
            
        protected:
            void HandleOKCallback();
            
        private:
            anitomyJs::AnitomyJs anitomy_;
    };
    
}

#endif