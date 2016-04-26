#include <nan.h>
#include <anitomy/anitomy.h>
#include <iostream>
#include <string>

void Test(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    anitomy::Anitomy anitomy;
    anitomy.Parse(L"[Ouroboros]_Fullmetal_Alchemist_Brotherhood_-_01.mkv");  

    auto& elements = anitomy.elements();

    for (auto& element : elements) {
        std::wcout << element.first << L"\t" << element.second << std::endl;
    }
    std::wcout << std::endl;

    std::wstring ws(elements.get(anitomy::kElementAnimeTitle).c_str());
    std::string str(ws.begin(), ws.end());
    
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, str.c_str()));
}

void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("test").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Test)->GetFunction());
}

NODE_MODULE(anitomy, Init)  