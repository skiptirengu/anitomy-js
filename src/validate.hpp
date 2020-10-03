#pragma once

#include <napi.h>

namespace anitomy_js {

// external validation functions
const char *ParseAndValidate(const Napi::CallbackInfo &args, Napi::Value &input, Napi::Object &options);

} // namespace anitomy_js