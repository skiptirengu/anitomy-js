/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <nan.h>
#include "worker.h"

namespace anitomyJs {
    
    bool ValidateInput(v8::Local<v8::Value> value, v8::Isolate* isolate) {
        bool valid = value->IsString() || value->IsArray();
        if (!valid) isolate->ThrowException(
                             v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong data type")));
        return valid;
    }
    
    void ParseSync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Value> input = args[0];
        
        if (!ValidateInput(input, isolate)) return;
        
        anitomyJs::AnitomyJs anitomy;
        anitomy.SetInput(input);
        anitomy.Parse();
        
        args.GetReturnValue().Set(anitomy.ParsedResult(isolate));
    }

    void ParseAsync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Value> input = args[0];
        
        if (!ValidateInput(input, isolate)) return;
        if (!args[1]->IsFunction()) {
            isolate->ThrowException(
                     v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Second parameter must be a callback")));
            return;            
        } 
        
        Nan::Callback* callback = new Nan::Callback(args[1].As<v8::Function>());
        anitomyJs::Worker* worker = new anitomyJs::Worker(callback);
        worker->GetAnitomy()->SetInput(input);
        Nan::AsyncQueueWorker(worker);
        
        args.GetReturnValue().Set(Nan::Undefined());
    }

    void Init(v8::Local<v8::Object> exports) {  
        exports->Set(Nan::New("parseSync").ToLocalChecked(),
                    Nan::New<v8::FunctionTemplate>(ParseSync)->GetFunction());
                    
        exports->Set(Nan::New("parseAsync").ToLocalChecked(),
                    Nan::New<v8::FunctionTemplate>(ParseAsync)->GetFunction());
    }
    
    NODE_MODULE(anitomy, Init)  
    
    // TODO stuff 
    // Create some tests
}