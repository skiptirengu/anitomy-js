#include "validate.hpp"
#include "util.hpp"

namespace anitomy_js {

using Napi::CallbackInfo;
using Napi::Env;

const char *ParseAndValidate(const Napi::CallbackInfo &args, Napi::Value &input, Napi::Object &options) {
  auto env = args.Env();
  auto len = args.Length();
  auto has_input = len > 0;
  auto has_options = len > 1;

  if (!has_input) {
    return "Input must be either a string or array";
  }

  input = args[0];

  if (!input.IsString() && !input.IsArray()) {
    return "Input must be either a string or array";
  }

  if (!has_options) {
    return NULL;
  }

  options = args[1].As<Napi::Object>();

  if (!options.IsObject()) {
    return NULL;
  }

  auto allowed_delimiters = "allowed_delimiters";
  auto ignored_strings = "ignored_strings";

  if (options.Has(allowed_delimiters) && !options.Get(allowed_delimiters).IsString()) {
    options.Set(allowed_delimiters, env.Undefined());
  }

  if (options.Has(ignored_strings) && !options.Get(ignored_strings).IsArray()) {
    options.Set(ignored_strings, env.Undefined());
  }

  return NULL;
}

} // namespace anitomy_js