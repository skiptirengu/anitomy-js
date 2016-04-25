#include <nan.h>
#include <anitomy/anitomy.h>

void Test(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    anitomy::Anitomy anitomy;
    anitomy.Parse(L"[Ouroboros]_Fullmetal_Alchemist_Brotherhood_-_01.mkv");  

    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "Hello world!"));
}

void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("test").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Test)->GetFunction());
}

NODE_MODULE(anitomy, Init)  

/**
TODO Add these to bindings.gyp
"anitomy.cpp",
"anitomy.h",
"element.cpp",
"element.h",
"keyword.cpp",
"keyword.h",
"options.h",
"parser.cpp",
"parser.h",
"parser_helper.c",
"parser_number.c",
"string.cpp",
"string.h",
"token.cpp",
"token.h",
"tokenizer.cpp",
"tokenizer.h"
**/