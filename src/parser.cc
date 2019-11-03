#include "parser.hpp"
#include "util.hpp"

namespace anitomy_js {

using anitomy::ElementCategory;
using anitomy::Elements;
using anitomy::string_t;
using Nan::EscapableHandleScope;
using Nan::Get;
using Nan::New;
using Nan::Set;
using Nan::To;
using v8::Array;
using v8::Local;
using v8::Object;
using v8::Value;

Local<Value> Parser::Parse(Local<Value> input_val, Local<Value> options_val) {
  // anitomy::Anitomy parser;

  auto batch = input_val->IsArray();
  auto input = Local<Array>::Cast(input_val);

  Local<Object> options = To<Object>(options_val).ToLocalChecked();
  // auto &anitomy_options = parser.options();

  auto length = input->Length();
  Local<Array> result = New<Array>(length);

  for (unsigned int i = 0; i < length; i++) {
    auto actual = Get(input, i).ToLocalChecked();
    auto current = To<String>(actual).ToLocalChecked();
    auto str = node_string_to_wstring(current);
    // parser.Parse(str);

    // auto &elements = parser.elements();
    // auto object = BuildObject(elements);
    // Set(result, i, object);
  }

  return batch ? Local<Value>::Cast(result) : Get(result, 0).ToLocalChecked();
}

void Parser::AddKey(Local<Object> &object, const char *entry, Elements &elements, ElementCategory pos) {
  auto name = New(entry).ToLocalChecked();

  switch (elements.count(pos)) {
  case 0:
    break;
  case 1: {
    auto value = elements.get(pos);
    Set(object, name, wstring_to_node_string(value));
    break;
  }
  default:
    Set(object, name, MultipleElements(elements, pos));
  }
}

Local<Array> Parser::MultipleElements(Elements &elements, ElementCategory pos) {
  auto array = elements.get_all(pos);
  auto output = New<Array>(array.size());

  unsigned int index = 0;
  for (string_t value : array) {
    Set(output, index, wstring_to_node_string(value));
    index++;
  }

  return output;
}

Local<Object> Parser::BuildObject(Elements &elements) {
  EscapableHandleScope scope;
  auto object = New<Object>();

  AddKey(object, "anime_season", elements, ElementCategory::kElementAnimeSeason);
  AddKey(object, "season_prefix", elements, ElementCategory::kElementAnimeSeasonPrefix);
  AddKey(object, "anime_title", elements, ElementCategory::kElementAnimeTitle);
  AddKey(object, "anime_type", elements, ElementCategory::kElementAnimeType);
  AddKey(object, "anime_year", elements, ElementCategory::kElementAnimeYear);
  AddKey(object, "audio_term", elements, ElementCategory::kElementAudioTerm);
  AddKey(object, "device_compatibility", elements, ElementCategory::kElementDeviceCompatibility);
  AddKey(object, "episode_number", elements, ElementCategory::kElementEpisodeNumber);
  AddKey(object, "episode_number_alt", elements, ElementCategory::kElementEpisodeNumberAlt);
  AddKey(object, "episode_prefix", elements, ElementCategory::kElementEpisodePrefix);
  AddKey(object, "episode_title", elements, ElementCategory::kElementEpisodeTitle);
  AddKey(object, "file_checksum", elements, ElementCategory::kElementFileChecksum);
  AddKey(object, "file_extension", elements, ElementCategory::kElementFileExtension);
  AddKey(object, "file_name", elements, ElementCategory::kElementFileName);
  AddKey(object, "language", elements, ElementCategory::kElementLanguage);
  AddKey(object, "other", elements, ElementCategory::kElementOther);
  AddKey(object, "release_group", elements, ElementCategory::kElementReleaseGroup);
  AddKey(object, "release_information", elements, ElementCategory::kElementReleaseInformation);
  AddKey(object, "release_version", elements, ElementCategory::kElementReleaseVersion);
  AddKey(object, "source", elements, ElementCategory::kElementSource);
  AddKey(object, "subtitles", elements, ElementCategory::kElementSubtitles);
  AddKey(object, "video_resolution", elements, ElementCategory::kElementVideoResolution);
  AddKey(object, "video_term", elements, ElementCategory::kElementVideoTerm);
  AddKey(object, "volume_number", elements, ElementCategory::kElementVolumeNumber);
  AddKey(object, "volume_prefix", elements, ElementCategory::kElementVolumePrefix);
  AddKey(object, "unknown", elements, ElementCategory::kElementUnknown);

  return scope.Escape(object);
}

} // namespace anitomy_js
