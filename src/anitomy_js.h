/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ANITOMY_ANITOMY_JS_H
#define ANITOMY_ANITOMY_JS_H

#include <anitomy/anitomy.h>
#include <nan.h>
#include <string>

namespace anitomyJs {
    
    class AnitomyJs {
        public:
            void SetIsolate(v8::Isolate* isolate);
            v8::Isolate* GetIsolate();
            
            v8::Local<v8::Object> Parse(v8::Local<v8::String> value);
            v8::Local<v8::Array> Parse(v8::Local<v8::Array> value);
            
        private:
            v8::Isolate* isolate_;
            anitomy::Anitomy anitomy_;
        
            std::wstring ToWideString(v8::Local<v8::Value> str);
            std::string ToStr(anitomy::string_t str);
            
            v8::Local<v8::Object> BuildObject(anitomy::Elements& elements);
            void SetEntry(v8::Local<v8::Object>& object, const char* entry,
                          anitomy::Elements& elements, anitomy::ElementCategory pos);
                          
    };
    
}

#endif