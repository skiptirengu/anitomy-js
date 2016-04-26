#include <nan.h>
#include <anitomy/anitomy.h>
#include <iostream>

void Test(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    anitomy::Anitomy anitomy;
    anitomy.Parse(L"[Ouroboros]_Fullmetal_Alchemist_Brotherhood_-_01.mkv");  

    auto& elements = anitomy.elements();

    for (auto& element : elements) {
        std::wcout << element.first << L"\t" << element.second << std::endl;
    }
    std::wcout << std::endl;

    // You can access values directly by using get() and get_all() methods
    std::wcout << elements.get(anitomy::kElementAnimeTitle)    << L" #"   <<
                  elements.get(anitomy::kElementEpisodeNumber) << L" by " <<
                  elements.get(anitomy::kElementReleaseGroup)  << std::endl;
                
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "Hello world!"));
}

void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("test").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Test)->GetFunction());
}

NODE_MODULE(anitomy, Init)  