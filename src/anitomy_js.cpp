/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * Copyright (c) 2016 Thiago Oliveira
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#include "anitomy_js.h"

// TODO tests for options!
namespace anitomyJs {
    
    void AnitomyJs::SetInput(v8::Local<v8::Value> value) {
        is_batch_parse_ = value->IsArray();
        if (is_batch_parse_) {
            v8::Local<v8::Array> input = v8::Local<v8::Array>::Cast(value);
            for (unsigned int i = 0; i < input->Length(); i++) input_.push_back(ToWideString(input->Get(i)));
        } else {
            input_.push_back(ToWideString(value));
        }
    }
    
    bool AnitomyJs::SetOptions(v8::Local<v8::Object> value, v8::Isolate* isolate) {
        v8::Local<v8::String> allowed_delimiters_str = v8::String::NewFromUtf8(isolate, "allowed_delimiters");
        v8::Local<v8::String> ignored_strings_str = v8::String::NewFromUtf8(isolate, "ignored_strings");
        anitomy::Options& anitomy_options = anitomy_.options();
        
        // Parse allowed_delimiters option
        if (value->Has(allowed_delimiters_str)) {
            v8::Local<v8::Value> allowed_delimiters = value->Get(allowed_delimiters_str);
            if (!allowed_delimiters->IsString()) {
                isolate->ThrowException(v8::Exception::TypeError(
                                        v8::String::NewFromUtf8(isolate, "allowed_delimiters must be a string")));
                return false;
            }
            anitomy_options.allowed_delimiters = ToWideString(allowed_delimiters);
        }
        
        // Parse ignored_strings option
        if (value->Has(ignored_strings_str)) {
            v8::Local<v8::Value> string_array = value->Get(ignored_strings_str);
            if (!string_array->IsArray()) {
                isolate->ThrowException(v8::Exception::TypeError(
                                        v8::String::NewFromUtf8(isolate, "ignored_strings must be an array")));
                return false;
            }
            v8::Local<v8::Array> ignored_strings = v8::Local<v8::Array>::Cast(string_array);
            std::vector<anitomy::string_t> strings(ignored_strings->Length());
            for (unsigned int i = 0; i < ignored_strings->Length(); i++) {
                v8::Local<v8::Value> ignored_string = ignored_strings->Get(i);
                if (!ignored_string->IsString()) {
                    isolate->ThrowException(v8::Exception::TypeError(
                                            v8::String::NewFromUtf8(isolate, "ignored_strings must be an array of strings")));
                    return false;
                }
                strings.push_back(ToWideString(ignored_string));
            }
            anitomy_options.ignored_strings = strings;
        }
        
        // Parse other options
        AddBoolOption("parse_episode_number", anitomy_options.parse_episode_number, value, isolate);
        AddBoolOption("parse_episode_title", anitomy_options.parse_episode_title, value, isolate);
        AddBoolOption("parse_file_extension", anitomy_options.parse_file_extension, value, isolate);
        AddBoolOption("parse_release_group", anitomy_options.parse_release_group, value, isolate);
    }
    
    void AnitomyJs::AddBoolOption(const char* name, bool& option, v8::Local<v8::Object> value, v8::Isolate* isolate) {
        v8::Local<v8::String> entry_name = v8::String::NewFromUtf8(isolate, name);
        option = value->Get(entry_name)->IsTrue();
    }
    
    void AnitomyJs::Parse() {
        for (std::wstring str : input_) {
            anitomy_.Parse(str);
            parsed_.push_back(anitomy_.elements());
        }
    }
    
    std::vector<anitomy::Elements> AnitomyJs::Parsed() {
        return parsed_;
    }
    
    // TODO For ~desu's sake, stop doing this flag shit
    v8::Local<v8::Value> AnitomyJs::ParsedResult(v8::Isolate* isolate) {
        v8::Local<v8::Array> output = v8::Array::New(isolate, parsed_.size());
        unsigned int index = 0;
        for (anitomy::Elements element : parsed_) {
            output->Set(index, BuildObject(element, isolate));
            index++;
        }
        if (is_batch_parse_) return output;
        return output->Get(0);
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
    
    void AnitomyJs::SetEntry(v8::Local<v8::Object>& object, v8::Isolate* isolate, const char* entry,
                             anitomy::Elements& elements, anitomy::ElementCategory pos) {
        v8::Local<v8::String> entry_name = v8::String::NewFromUtf8(isolate, entry);
        switch(elements.count(pos)) {
            case 0:
                break;
            case 1:
                object->Set(entry_name, v8::String::NewFromUtf8(isolate, ToStr(elements.get(pos)).c_str()));
                break;
            default:
                object->Set(entry_name, CategoryArray(elements, pos, isolate));
        }
    }
    
    v8::Local<v8::Array> AnitomyJs::CategoryArray(anitomy::Elements& elements, 
                                                  anitomy::ElementCategory pos, v8::Isolate* isolate) {
        std::vector<anitomy::string_t> category_elements = elements.get_all(pos);
        v8::Local<v8::Array> output = v8::Array::New(isolate, category_elements.size());
        unsigned int index = 0;
        for (anitomy::string_t value : category_elements) {
            output->Set(index, v8::String::NewFromUtf8(isolate, ToStr(value).c_str()));
            index++;
        }
        return output;
    }
    
    v8::Local<v8::Object> AnitomyJs::BuildObject(anitomy::Elements& elements, v8::Isolate* isolate) {
        v8::Local<v8::Object> object = v8::Object::New(isolate);
        SetEntry(object, isolate, "anime_season", elements, anitomy::kElementAnimeSeason);
        SetEntry(object, isolate, "season_prefix", elements, anitomy::kElementAnimeSeasonPrefix);
        SetEntry(object, isolate, "anime_title", elements, anitomy::kElementAnimeTitle);
        SetEntry(object, isolate, "anime_type", elements, anitomy::kElementAnimeType);
        SetEntry(object, isolate, "anime_year", elements, anitomy::kElementAnimeYear);
        SetEntry(object, isolate, "audio_term", elements, anitomy::kElementAudioTerm);
        SetEntry(object, isolate, "device_compatibility", elements, anitomy::kElementDeviceCompatibility);
        SetEntry(object, isolate, "episode_number", elements, anitomy::kElementEpisodeNumber);
        SetEntry(object, isolate, "episode_number_alt", elements, anitomy::kElementEpisodeNumberAlt);
        SetEntry(object, isolate, "episode_prefix", elements, anitomy::kElementEpisodePrefix);
        SetEntry(object, isolate, "episode_title", elements, anitomy::kElementEpisodeTitle);
        SetEntry(object, isolate, "file_checksum", elements, anitomy::kElementFileChecksum);
        SetEntry(object, isolate, "file_extension", elements, anitomy::kElementFileExtension);
        SetEntry(object, isolate, "file_name", elements, anitomy::kElementFileName);
        SetEntry(object, isolate, "language", elements, anitomy::kElementLanguage);
        SetEntry(object, isolate, "other", elements, anitomy::kElementOther);
        SetEntry(object, isolate, "release_group", elements, anitomy::kElementReleaseGroup);
        SetEntry(object, isolate, "release_information", elements, anitomy::kElementReleaseInformation);
        SetEntry(object, isolate, "release_version", elements, anitomy::kElementReleaseVersion);
        SetEntry(object, isolate, "source", elements, anitomy::kElementSource);
        SetEntry(object, isolate, "subtitles", elements, anitomy::kElementSubtitles);
        SetEntry(object, isolate, "video_resolution", elements, anitomy::kElementVideoResolution);
        SetEntry(object, isolate, "video_term", elements, anitomy::kElementVideoTerm);
        SetEntry(object, isolate, "volume_number", elements, anitomy::kElementVolumeNumber);
        SetEntry(object, isolate, "volume_prefix", elements, anitomy::kElementVolumePrefix);
        SetEntry(object, isolate, "unknown", elements, anitomy::kElementUnknown);
        return object;
    }
    
}
