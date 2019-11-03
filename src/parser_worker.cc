#include "parser_worker.hpp"
#include "util.hpp"

namespace anitomy_js {

using Nan::AsyncWorker;
using Nan::Get;
using Nan::HandleScope;
using Nan::Null;
using Nan::Undefined;
using v8::Local;
using v8::Value;

void ParserWorker::Execute() { this->_parser.Parse(); }

void ParserWorker::HandleOKCallback() {
  HandleScope scope;
  auto output = this->_parser.Result();
  Local<Value> argv[] = {
      Null(),
      output,
  };
  callback->Call(2, argv, async_resource);
}

} // namespace anitomy_js