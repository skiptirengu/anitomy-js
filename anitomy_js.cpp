#include <nan.h>
#include <uv.h>
#include <unistd.h>
#include <anitomy/anitomy.h>
#include <string>

namespace anitomy_js {
    
    struct Work {
        uv_work_t request;
        v8::Persistent<v8::Function> callback;
    };
    
    static void DoParseAsync(uv_work_t* req) {
        Work *work = static_cast<Work*>(req->data);
    }
    
    static void DoParseAsyncComplete(uv_work_t* req, int status) {
        
    }
    
    std::wstring ToWideString(v8::Local<v8::Value> str) {
        v8::String::Utf8Value utf_value(str->ToString());
        std::string str_value(*utf_value);
        return std::wstring(str_value.begin(), str_value.end());
    }
    
    std::string ToStr(anitomy::string_t str) {
        std::wstring ws_value(str.c_str());
        return std::string(ws_value.begin(), ws_value.end());
    }
    
    void SetEntry(v8::Local<v8::Object>& object, v8::Isolate* isolate, const char* entry, anitomy::Elements& elements, anitomy::ElementCategory pos) {
        object->Set(v8::String::NewFromUtf8(isolate, entry), 
                    v8::String::NewFromUtf8(isolate, ToStr(elements.get(pos)).c_str()));
    }
    
    v8::Local<v8::Object> BuildObject(anitomy::Elements& elements, v8::Isolate* isolate) {
        v8::Local<v8::Object> object = v8::Object::New(isolate);
        
        SetEntry(object, isolate, "title", elements, anitomy::kElementAnimeTitle);
        SetEntry(object, isolate, "episode_number", elements, anitomy::kElementEpisodeNumber);
        SetEntry(object, isolate, "episode_prefix", elements, anitomy::kElementEpisodePrefix);
        SetEntry(object, isolate, "episode_title", elements, anitomy::kElementEpisodeTitle);
        SetEntry(object, isolate, "file_extension", elements, anitomy::kElementFileExtension);
        SetEntry(object, isolate, "file_name", elements, anitomy::kElementFileName);
        SetEntry(object, isolate, "language", elements, anitomy::kElementLanguage);
        SetEntry(object, isolate, "release_group", elements, anitomy::kElementReleaseGroup);
        SetEntry(object, isolate, "release_version", elements, anitomy::kElementReleaseVersion);
        SetEntry(object, isolate, "subtitles", elements, anitomy::kElementSubtitles);
        SetEntry(object, isolate, "video_resolution", elements, anitomy::kElementVideoResolution);
        
        return object;
    }
    
    // v8::Value DoParse(v8::Isolate* isolate, v8::Value& input, bool is_string) {
    //     v8::Local<v8::Array> output_array = v8::Array::New(isolate);
    //     v8::Local<v8::Array> input_array = v8::Array::New(isolate);
    // }
    
    void ParseSync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();

        if (args.Length() < 1) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }
        
        if (!args[0]->IsString()) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong data type")));
            return;
        } 
        
        anitomy::Anitomy anitomy;
        anitomy.Parse(ToWideString(args[0]));
        
        args.GetReturnValue().Set(BuildObject(anitomy.elements(), isolate));
    }

    void ParseAsync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate* isolate = args.GetIsolate();
     
        if (args.Length() < 2) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }   
        
        if (!args[0]->IsArray() && !args[0]->IsString()) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong data type")));
            return;
        }
               
        Work* work = new Work();
        work->request.data = work;
        
        v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[1]);
        work->callback.Reset(isolate, callback);
        
        uv_queue_work(uv_default_loop(), &work->request, DoParseAsync, DoParseAsyncComplete);
        
        args.GetReturnValue().Set(Undefined(isolate));  
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