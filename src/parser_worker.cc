#include "parser_worker.hpp"
#include "util.hpp"

namespace anitomy_js {

void ParserWorker::Execute() { this->_parser.Parse(); }

void ParserWorker::OnOK() {
  auto result = _parser.Result();
  _deferred.Resolve(result);
}

void ParserWorker::OnError(Napi::Error const &error) {
  auto result = error.Value();
  _deferred.Reject(result);
}

Napi::Promise ParserWorker::GetPromise() { return _deferred.Promise(); }

} // namespace anitomy_js