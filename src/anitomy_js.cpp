/*
** Copyright (c) 2016, Thiago Oliveira
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_js.h"

namespace anitomyJs {
    
    void AnitomyJs::SetIsolate(v8::Isolate* isolate) {
        isolate_ = isolate;
    }
    
    v8::Isolate* AnitomyJs::GetIsolate() {
        return isolate_;
    }
    
    v8::Local<v8::Object> AnitomyJs::Parse(v8::Local<v8::String> value) {
        anitomy_.Parse(ToWideString(value));
        return BuildObject(anitomy_.elements());
    }
    
    v8::Local<v8::Array> AnitomyJs::Parse(v8::Local<v8::Array> value) {
        unsigned int length = value->Length();
        v8::Local<v8::Array> result_list = v8::Array::New(isolate_, length);
        
        for (unsigned int i = 0; i < length; i++) {
            anitomy_.Parse(ToWideString(value->Get(i)));
            v8::Local<v8::Object> object = BuildObject(anitomy_.elements());
            result_list->Set(i, object);
        }
        
        return result_list;
    }
    
    std::wstring AnitomyJs::ToWideString(v8::Local<v8::Value> str) {
        v8::String::Utf8Value utf_value(str->ToString());
        std::string str_value(*utf_value);
        return std::wstring(str_value.begin(), str_value.end());
    }
    
    std::string AnitomyJs::ToStr(anitomy::string_t str) {
        std::wstring ws_value(str.c_str());
        return std::string(ws_value.begin(), ws_value.end());
    }
    
    void AnitomyJs::SetEntry(v8::Local<v8::Object>& object, const char* entry,
                             anitomy::Elements& elements, anitomy::ElementCategory pos) {
        object->Set(v8::String::NewFromUtf8(isolate_, entry), 
                    v8::String::NewFromUtf8(isolate_, ToStr(elements.get(pos)).c_str()));;
    }
    
    v8::Local<v8::Object> AnitomyJs::BuildObject(anitomy::Elements& elements) {
        v8::Local<v8::Object> object = v8::Object::New(isolate_);
        SetEntry(object, "anime_season", elements, anitomy::kElementAnimeSeason);
        SetEntry(object, "season_prefix", elements, anitomy::kElementAnimeSeasonPrefix);
        SetEntry(object, "anime_title", elements, anitomy::kElementAnimeTitle);
        SetEntry(object, "anime_type", elements, anitomy::kElementAnimeType);
        SetEntry(object, "anime_year", elements, anitomy::kElementAnimeYear);
        SetEntry(object, "audio_term", elements, anitomy::kElementAudioTerm);
        SetEntry(object, "device_compatibility", elements, anitomy::kElementDeviceCompatibility);
        SetEntry(object, "episode_number", elements, anitomy::kElementEpisodeNumber);
        SetEntry(object, "episode_number_alt", elements, anitomy::kElementEpisodeNumberAlt);
        SetEntry(object, "episode_prefix", elements, anitomy::kElementEpisodePrefix);
        SetEntry(object, "episode_title", elements, anitomy::kElementEpisodeTitle);
        SetEntry(object, "file_checksum", elements, anitomy::kElementFileChecksum);
        SetEntry(object, "file_extension", elements, anitomy::kElementFileExtension);
        SetEntry(object, "file_name", elements, anitomy::kElementFileName);
        SetEntry(object, "language", elements, anitomy::kElementLanguage);
        SetEntry(object, "other", elements, anitomy::kElementOther);
        SetEntry(object, "release_group", elements, anitomy::kElementReleaseGroup);
        SetEntry(object, "release_information", elements, anitomy::kElementReleaseInformation);
        SetEntry(object, "release_version", elements, anitomy::kElementReleaseVersion);
        SetEntry(object, "source", elements, anitomy::kElementSource);
        SetEntry(object, "subtitles", elements, anitomy::kElementSubtitles);
        SetEntry(object, "video_resolution", elements, anitomy::kElementVideoResolution);
        SetEntry(object, "video_term", elements, anitomy::kElementVideoTerm);
        SetEntry(object, "volume_number", elements, anitomy::kElementVolumeNumber);
        SetEntry(object, "volume_prefix", elements, anitomy::kElementVolumePrefix);
        SetEntry(object, "unknow", elements, anitomy::kElementUnknown);
        return object;
    }
}