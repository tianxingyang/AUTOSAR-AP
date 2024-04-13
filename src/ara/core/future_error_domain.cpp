#include "ara/core/future_error_domain.h"

namespace ara::core {

const char* FutureErrorDomain::Message(CodeType error_code) const noexcept {
  switch (const auto code{static_cast<Errc>(error_code)}) {
    case future_errc::broken_promise:
      return "Broken promise";
    case future_errc::future_already_retrieved:
      return "Future already retrieved";
    case future_errc::no_state:
      return "No state";
    case future_errc::promise_already_satisfied:
      return "Promise already satisfied";
    default:
      return "Unknown error";
  }
}

void FutureErrorDomain::ThrowAsException(const ErrorCode& error_code) const noexcept(false) {
  throw Exception{error_code};
}
}  // namespace ara::core
