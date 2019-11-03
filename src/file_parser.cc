#include "file_parser.hpp"
#include "util.hpp"

namespace anitomy_js {

using anitomy::Anitomy;
using anitomy::ElementCategory;
using anitomy::Elements;
using anitomy::string_t;
using Nan::EscapableHandleScope;
using Nan::Get;
using Nan::New;
using Nan::Set;
using Nan::To;
using std::size_t;
using v8::Array;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

FileParser::FileParser(Local<Value> input, Local<Value> options) {
  this->_batch = input->IsArray();

  unsigned int length;
  auto object = this->CoerceArray(input, length);

  for (unsigned int i = 0; i < length; i++) {
    auto actual = Get(object, i).ToLocalChecked();
    auto current = To<String>(actual).ToLocalChecked();
    this->_input.emplace_back(node_string_to_wstring(current));
  }
}

Local<Object> FileParser::CoerceArray(Local<Value> value, unsigned int &length) {
  if (value->IsArray()) {
    length = Local<Array>::Cast(value)->Length();
    return To<Object>(value).ToLocalChecked();
  } else {
    length = 1;
    auto array = New<Array>(length);
    Set(array, 0, value);
    return To<Object>(array).ToLocalChecked();
  }
}

void FileParser::Parse() {
  Anitomy parser;

  // Local<Object> options = To<Object>(options_val).ToLocalChecked();
  // auto &anitomy_options = parser.options();

  // auto length = input->Length();
  // Local<Array> result = New<Array>(length);

  for (const auto value : this->_input) {
    parser.Parse(value);
    auto elements = parser.elements();
    this->_output.emplace_back(elements);
  }
}

Local<Value> FileParser::Result() {
  auto length = this->_output.size();
  Local<Array> result = New<Array>(length);

  for (size_t i = 0; i < length; i++) {
    const auto &elements = this->_output[i];
    const auto object = BuildObject(elements);
    Set(result, i, object);
  }

  return this->_batch ? Local<Value>::Cast(result) : Get(result, 0).ToLocalChecked();
}

void FileParser::AddKey(Local<Object> &object, const char *entry, const Elements &elements, ElementCategory pos) {
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
    Set(object, name, this->MultipleElements(elements, pos));
  }
}

Local<Array> FileParser::MultipleElements(const Elements &elements, ElementCategory pos) {
  auto array = elements.get_all(pos);
  auto output = New<Array>(array.size());

  unsigned int index = 0;
  for (string_t value : array) {
    Set(output, index, wstring_to_node_string(value));
    index++;
  }

  return output;
}

Local<Object> FileParser::BuildObject(const Elements &elements) {
  auto object = New<Object>();
  this->AddKey(object, "anime_season", elements, ElementCategory::kElementAnimeSeason);
  this->AddKey(object, "season_prefix", elements, ElementCategory::kElementAnimeSeasonPrefix);
  this->AddKey(object, "anime_title", elements, ElementCategory::kElementAnimeTitle);
  this->AddKey(object, "anime_type", elements, ElementCategory::kElementAnimeType);
  this->AddKey(object, "anime_year", elements, ElementCategory::kElementAnimeYear);
  this->AddKey(object, "audio_term", elements, ElementCategory::kElementAudioTerm);
  this->AddKey(object, "device_compatibility", elements, ElementCategory::kElementDeviceCompatibility);
  this->AddKey(object, "episode_number", elements, ElementCategory::kElementEpisodeNumber);
  this->AddKey(object, "episode_number_alt", elements, ElementCategory::kElementEpisodeNumberAlt);
  this->AddKey(object, "episode_prefix", elements, ElementCategory::kElementEpisodePrefix);
  this->AddKey(object, "episode_title", elements, ElementCategory::kElementEpisodeTitle);
  this->AddKey(object, "file_checksum", elements, ElementCategory::kElementFileChecksum);
  this->AddKey(object, "file_extension", elements, ElementCategory::kElementFileExtension);
  this->AddKey(object, "file_name", elements, ElementCategory::kElementFileName);
  this->AddKey(object, "language", elements, ElementCategory::kElementLanguage);
  this->AddKey(object, "other", elements, ElementCategory::kElementOther);
  this->AddKey(object, "release_group", elements, ElementCategory::kElementReleaseGroup);
  this->AddKey(object, "release_information", elements, ElementCategory::kElementReleaseInformation);
  this->AddKey(object, "release_version", elements, ElementCategory::kElementReleaseVersion);
  this->AddKey(object, "source", elements, ElementCategory::kElementSource);
  this->AddKey(object, "subtitles", elements, ElementCategory::kElementSubtitles);
  this->AddKey(object, "video_resolution", elements, ElementCategory::kElementVideoResolution);
  this->AddKey(object, "video_term", elements, ElementCategory::kElementVideoTerm);
  this->AddKey(object, "volume_number", elements, ElementCategory::kElementVolumeNumber);
  this->AddKey(object, "volume_prefix", elements, ElementCategory::kElementVolumePrefix);
  this->AddKey(object, "unknown", elements, ElementCategory::kElementUnknown);
  return object;
}

} // namespace anitomy_js
