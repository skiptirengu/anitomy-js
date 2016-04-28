/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

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
            void Parse();
            
            std::vector<anitomy::Elements> Parsed();
            v8::Local<v8::Value> ParsedResult(v8::Isolate* isolate);
            
        private:
            anitomy::Anitomy anitomy_;
            std::vector<std::wstring> input_;
            
            // Bad, bad programmer
            std::vector<anitomy::Elements> parsed_; // <---- Please don't do this
            bool is_batch_parse_;                   // <---- and this...
        
            std::wstring ToWideString(v8::Local<v8::Value> str);
            std::string ToStr(anitomy::string_t str);
            
            v8::Local<v8::Object> BuildObject(anitomy::Elements& elements, v8::Isolate* isolate);
            void SetEntry(v8::Local<v8::Object>& object, v8::Isolate* isolate, const char* entry,
                          anitomy::Elements& elements, anitomy::ElementCategory pos);
            v8::Local<v8::Array> CategoryArray(anitomy::Elements& elements, 
                                               anitomy::ElementCategory pos, v8::Isolate* isolate);
    };
    
}

#endif