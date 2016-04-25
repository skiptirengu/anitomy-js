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