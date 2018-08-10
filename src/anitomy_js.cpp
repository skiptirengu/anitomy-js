/*********************************************************************
 * AnitomyJs - Node.js wrapper for Anitomy
 *
 * MIT License <https://github.com/nevermnd/anitomy-js/blob/master/LICENSE>
 ********************************************************************/

#include "anitomy_js.h"

namespace anitomyJs {

void AnitomyJs::SetInput(Local<Value> value, Isolate *isolate) {
  is_batch_parse_ = value->IsArray();
  if (is_batch_parse_) {
    Local<Array> input = Local<Array>::Cast(value);
    for (unsigned int i = 0; i < input->Length(); i++) {
      input_.push_back(ToWideString(input->Get(i), isolate));
    }
  } else {
    input_.push_back(ToWideString(value, isolate));
  }
}

bool AnitomyJs::SetOptions(Local<Object> value, Isolate *isolate) {
  Local<String> allowed_delimiters_str = String::NewFromUtf8(isolate, "allowed_delimiters");
  Local<String> ignored_strings_str = String::NewFromUtf8(isolate, "ignored_strings");
  Options &anitomy_options = anitomy_.options();

  // Parse allowed_delimiters option
  if (value->Has(allowed_delimiters_str)) {
    Local<Value> allowed_delimiters = value->Get(allowed_delimiters_str);
    if (!allowed_delimiters->IsString()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "allowed_delimiters must be a string")));
      return false;
    }
    anitomy_options.allowed_delimiters = ToWideString(allowed_delimiters, isolate);
  }

  // Parse ignored_strings option
  if (value->Has(ignored_strings_str)) {
    Local<Value> string_array = value->Get(ignored_strings_str);
    if (!string_array->IsArray()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "ignored_strings must be an array")));
      return false;
    }
    Local<Array> ignored_strings = Local<Array>::Cast(string_array);
    unsigned int ignored_strings_length = ignored_strings->Length();
    vector<string_t> strings(ignored_strings_length);
    for (unsigned int i = 0; i < ignored_strings_length; i++) {
      strings.push_back(ToWideString(ignored_strings->Get(i)->ToString(), isolate));
    }
    anitomy_options.ignored_strings = strings;
  }

  // other options
  anitomy_options.parse_episode_number = BoolOption("parse_episode_number", value, isolate);
  anitomy_options.parse_episode_title = BoolOption("parse_episode_title", value, isolate);
  anitomy_options.parse_file_extension = BoolOption("parse_file_extension", value, isolate);
  anitomy_options.parse_release_group = BoolOption("parse_release_group", value, isolate);
  return true;
}

bool AnitomyJs::BoolOption(const char *name, Local<Object> value, Isolate *isolate) {
  Local<String> entry_name = String::NewFromUtf8(isolate, name);
  return value->Has(entry_name) ? value->Get(entry_name)->ToBoolean()->IsTrue() : true;
}

void AnitomyJs::Parse() {
  for (wstring str : input_) {
    anitomy_.Parse(str);
    parsed_.push_back(anitomy_.elements());
  }
}

vector<Elements> AnitomyJs::Parsed() { return parsed_; }

Local<Value> AnitomyJs::ParsedResult(Isolate *isolate) {
  Local<Array> output = Array::New(isolate, parsed_.size());
  unsigned int index = 0;
  for (Elements element : parsed_) {
    output->Set(index, BuildObject(element, isolate));
    index++;
  }
  if (is_batch_parse_) {
    return output;
  } else {
    return output->Get(0);
  }
}

wstring AnitomyJs::ToWideString(Local<Value> str, Isolate *isolate) {
#if NODE_MAJOR_VERSION >= 10
  String::Utf8Value utf_value(isolate, str->ToString());
#else
  String::Utf8Value utf_value(str->ToString());
#endif
  string str_value(*utf_value);
  return wstring(str_value.begin(), str_value.end());
}

string AnitomyJs::ToStr(string_t str) {
  wstring ws_value(str.c_str());
  return string(ws_value.begin(), ws_value.end());
}

void AnitomyJs::SetEntry(Local<Object> &object, Isolate *isolate, const char *entry, Elements &elements, ElementCategory pos) {
  Local<String> entry_name = String::NewFromUtf8(isolate, entry);
  switch (elements.count(pos)) {
  case 0:
    break;
  case 1:
    object->Set(entry_name, String::NewFromUtf8(isolate, ToStr(elements.get(pos)).c_str()));
    break;
  default:
    object->Set(entry_name, CategoryArray(elements, pos, isolate));
  }
}

Local<Array> AnitomyJs::CategoryArray(Elements &elements, ElementCategory pos, Isolate *isolate) {
  vector<string_t> category_elements = elements.get_all(pos);
  Local<Array> output = Array::New(isolate, category_elements.size());
  unsigned int index = 0;
  for (string_t value : category_elements) {
    output->Set(index, String::NewFromUtf8(isolate, ToStr(value).c_str()));
    index++;
  }
  return output;
}

Local<Object> AnitomyJs::BuildObject(Elements &elements, Isolate *isolate) {
  Local<Object> object = Object::New(isolate);
  SetEntry(object, isolate, "anime_season", elements, kElementAnimeSeason);
  SetEntry(object, isolate, "season_prefix", elements, kElementAnimeSeasonPrefix);
  SetEntry(object, isolate, "anime_title", elements, kElementAnimeTitle);
  SetEntry(object, isolate, "anime_type", elements, kElementAnimeType);
  SetEntry(object, isolate, "anime_year", elements, kElementAnimeYear);
  SetEntry(object, isolate, "audio_term", elements, kElementAudioTerm);
  SetEntry(object, isolate, "device_compatibility", elements, kElementDeviceCompatibility);
  SetEntry(object, isolate, "episode_number", elements, kElementEpisodeNumber);
  SetEntry(object, isolate, "episode_number_alt", elements, kElementEpisodeNumberAlt);
  SetEntry(object, isolate, "episode_prefix", elements, kElementEpisodePrefix);
  SetEntry(object, isolate, "episode_title", elements, kElementEpisodeTitle);
  SetEntry(object, isolate, "file_checksum", elements, kElementFileChecksum);
  SetEntry(object, isolate, "file_extension", elements, kElementFileExtension);
  SetEntry(object, isolate, "file_name", elements, kElementFileName);
  SetEntry(object, isolate, "language", elements, kElementLanguage);
  SetEntry(object, isolate, "other", elements, kElementOther);
  SetEntry(object, isolate, "release_group", elements, kElementReleaseGroup);
  SetEntry(object, isolate, "release_information", elements, kElementReleaseInformation);
  SetEntry(object, isolate, "release_version", elements, kElementReleaseVersion);
  SetEntry(object, isolate, "source", elements, kElementSource);
  SetEntry(object, isolate, "subtitles", elements, kElementSubtitles);
  SetEntry(object, isolate, "video_resolution", elements, kElementVideoResolution);
  SetEntry(object, isolate, "video_term", elements, kElementVideoTerm);
  SetEntry(object, isolate, "volume_number", elements, kElementVolumeNumber);
  SetEntry(object, isolate, "volume_prefix", elements, kElementVolumePrefix);
  SetEntry(object, isolate, "unknown", elements, kElementUnknown);
  return object;
}
} // namespace anitomyJs
