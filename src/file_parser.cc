#include "file_parser.hpp"
#include "util.hpp"

namespace anitomy_js {

using anitomy::ElementCategory;
using anitomy::Elements;
using anitomy::string_t;
using std::function;
using std::size_t;

FileParser::FileParser(Napi::Env env, Napi::Value input, Napi::Value options) : _env(env) {
  this->_batch = input.IsArray();

  // convert node strings to string_t
  MapNodeArray(this->CoerceArray(input), [this](const Napi::Value current) {
    auto str = node_string_to_wstring(current);
    this->_input.emplace_back(str);
  });

  auto &an_options = this->_anitomy.options();
  // set vector options
  this->SetStringVectorOption(options, "ignored_strings", an_options.ignored_strings);
  // set string options
  this->SetStringOption(options, "allowed_delimiters", an_options.allowed_delimiters);
  // set boolean options
  this->SetBoolOption(options, "parse_release_group", an_options.parse_release_group);
  this->SetBoolOption(options, "parse_file_extension", an_options.parse_file_extension);
  this->SetBoolOption(options, "parse_episode_title", an_options.parse_episode_title);
  this->SetBoolOption(options, "parse_episode_number", an_options.parse_episode_number);
}

void FileParser::SetStringVectorOption(Napi::Value options, const char *key, vector<string_t> &val) {
  val.clear();
  this->SafeSetOption(options, key, [&val](auto const value) {
    MapNodeArray(value, [&val](const auto curr) {
      auto str = node_string_to_wstring(curr);
      val.emplace_back(str);
    });
  });
}

void FileParser::SetStringOption(Napi::Value options, const char *key, string_t &val) {
  this->SafeSetOption(options, key, [&val](auto const value) {
    if (value.IsString()) {
      val = node_string_to_wstring(value);
    }
  });
}

void FileParser::SetBoolOption(Napi::Value options, const char *key, bool &val) {
  this->SafeSetOption(options, key, [&val](const Napi::Value value) {
    if (value.IsBoolean()) {
      val = value.ToBoolean().Value();
    }
  });
}

void FileParser::SafeSetOption(Napi::Value options, const char *key,
                               function<void(Napi::Value)> callback) {
  if (!options.IsObject()) {
    return;
  }
  auto object = options.ToObject();
  auto node_key = node_string(this->_env, key);
  if (!object.Has(node_key)) {
    return;
  }
  auto value = object.Get(node_key);
  callback(value);
}

Napi::Value FileParser::CoerceArray(Napi::Value value) {
  if (value.IsArray()) {
    return value;
  } else {
    auto array = Napi::Array::New(this->_env, 1);
    array[(uint32_t)0] = value;
    array.Set(Napi::Number::New(this->_env, 0.0), value);
    return array;
  }
}

void FileParser::Parse() {
  for (const auto value : this->_input) {
    this->_anitomy.Parse(value);
    auto elements = this->_anitomy.elements();
    this->_output.emplace_back(elements);
  }
}

Napi::Value FileParser::Result() {
  auto length = this->_output.size();
  Napi::Array result = Napi::Array::New(this->_env, length);
  for (size_t i = 0; i < length; i++) {
    const auto &elements = this->_output[i];
    const auto object = BuildObject(elements);
    result[i] = object;
  }
  return this->_batch ? result.As<Napi::Value>() : result[(uint32_t)0];
}

void FileParser::AddKey(Napi::Object &object, const char *entry, const Elements &elements,
                        ElementCategory pos) {
  auto name = node_string(this->_env, entry);
  switch (elements.count(pos)) {
  case 0:
    break;
  case 1: {
    auto value = elements.get(pos);
    object.Set(name, wstring_to_node_string(this->_env, value));
    break;
  }
  default:
    object.Set(name, this->MultipleElements(elements, pos));
  }
}

Napi::Array FileParser::MultipleElements(const Elements &elements, ElementCategory pos) {
  auto array = elements.get_all(pos);
  auto output = Napi::Array::New(this->_env, array.size());
  unsigned int index = 0;
  for (string_t value : array) {
    output[index] = wstring_to_node_string(this->_env, value);
    index++;
  }
  return output;
}

Napi::Object FileParser::BuildObject(const Elements &elements) {
  auto object = Napi::Object::New(this->_env);
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
