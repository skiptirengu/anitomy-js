/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#ifndef ANITOMY_JS_ANITOMY_JS_H_
#define ANITOMY_JS_ANITOMY_JS_H_

#include <anitomy/anitomy.h>
#include <nan.h>
#include <string>
#include <vector>

namespace anitomyJs {
    
    class AnitomyJs {
        public:
            void SetInput(v8::Local<v8::Value> value);
            bool SetOptions(v8::Local<v8::Object> value, v8::Isolate* isolate);
            void Parse();
            
            std::vector<anitomy::Elements> Parsed();
            v8::Local<v8::Value> ParsedResult(v8::Isolate* isolate);
            
        private:
            anitomy::Anitomy anitomy_;
            std::vector<std::wstring> input_;
            
            std::vector<anitomy::Elements> parsed_; 
            bool is_batch_parse_;                   
        
            std::wstring ToWideString(v8::Local<v8::Value> str);
            std::string ToStr(anitomy::string_t str);
            
            void AddBoolOption(const char* name, bool& option, v8::Local<v8::Object> value, v8::Isolate* isolate);
            
            v8::Local<v8::Object> BuildObject(anitomy::Elements& elements, v8::Isolate* isolate);
            void SetEntry(v8::Local<v8::Object>& object, v8::Isolate* isolate, const char* entry,
                          anitomy::Elements& elements, anitomy::ElementCategory pos);
            v8::Local<v8::Array> CategoryArray(anitomy::Elements& elements, 
                                               anitomy::ElementCategory pos, v8::Isolate* isolate);
    };
    
}

#endif