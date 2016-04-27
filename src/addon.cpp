/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <nan.h>
#include <anitomy/anitomy.h>
#include <string>
#include "anitomy_js.h"

namespace anitomy_js {
    
    void ParseSync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        v8::Local<v8::Value> value = args[0];
        
        anitomyJs::AnitomyJs anitomyJs;
        anitomyJs.SetIsolate(isolate);
        
        if (value->IsString()) {
            args.GetReturnValue().Set(anitomyJs.Parse(value->ToString()));    
        } else if (value->IsArray()) {
            args.GetReturnValue().Set(anitomyJs.Parse(v8::Local<v8::Array>::Cast(value)));
        } else {
            isolate->ThrowException(v8::Exception::TypeError(
                                    v8::String::NewFromUtf8(isolate, "Wrong data type")));
        }
    }

    void ParseAsync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "ParseAsync called"));
    }

    void Init(v8::Local<v8::Object> exports) {  
        exports->Set(Nan::New("parseSync").ToLocalChecked(),
                    Nan::New<v8::FunctionTemplate>(ParseSync)->GetFunction());
                    
        exports->Set(Nan::New("parseAsync").ToLocalChecked(),
                    Nan::New<v8::FunctionTemplate>(ParseAsync)->GetFunction());
    }
    
    NODE_MODULE(anitomy, Init)  
    
    // TODO stuff 
    // -> Finish async impl
    // -> Array input
    // -> Add all entries to ouput object
}