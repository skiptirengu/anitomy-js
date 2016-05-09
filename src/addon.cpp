/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#include <nan.h>
#include "worker.h"

namespace anitomyJs {
    
    bool ValidateInput(v8::Local<v8::Value> value, v8::Isolate* isolate) {
        bool valid = value->IsString() || value->IsArray();
        if (!valid) isolate->ThrowException(
                             v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong data type")));
        return valid;
    }
    
    bool ValidateOptions(v8::Local<v8::Value> options, v8::Isolate* isolate) {
        if (!options->IsObject()) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Options must be an object")));
            return false;
        }
        return true;
    }
    
    void ParseSync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        int args_length = args.Length();
        
        if (args_length < 1) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }
        
        v8::Local<v8::Value> input = args[0];
        if (!ValidateInput(input, isolate)) return;
        
        anitomyJs::AnitomyJs anitomy;
        if (args_length >= 2) {
            v8::Local<v8::Value> options = args[1];
            if (!ValidateOptions(options, isolate)) return;
            anitomy.SetOptions(options->ToObject(), isolate);
        }
        
        anitomy.SetInput(input);
        anitomy.Parse();
        
        args.GetReturnValue().Set(anitomy.ParsedResult(isolate));
    }

    void ParseAsync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
        int args_length = args.Length();
        
        if (args_length < 2) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }
        
        v8::Local<v8::Value> input = args[0];
        if (!ValidateInput(input, isolate)) return;
        if (!args[1]->IsFunction()) {
            isolate->ThrowException(
                     v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Second parameter must be a callback")));
            return;            
        }
        
        Nan::Callback* callback = new Nan::Callback(args[1].As<v8::Function>());
        anitomyJs::Worker* worker = new anitomyJs::Worker(callback);
        
        if (args_length >= 3) {
            v8::Local<v8::Value> options = args[2];
            if (!ValidateOptions(options, isolate)) return;
            worker->GetAnitomy()->SetOptions(options->ToObject(), isolate);
        }
        
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
    
}
