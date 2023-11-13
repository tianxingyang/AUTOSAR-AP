#include "ara/core/future_error_domain.h"

#include "ara/core/future.h"
#include "ara/core/promise.h"

namespace ara::core {
const char* FutureErrorDomain::Message(CodeType error_code) const noexcept {
  switch (const auto code{static_cast<Errc>(error_code)}) {
    case future_errc::kBrokenPromise:
      return "Broken promise";
    case future_errc::kFutureAlreadyRetrieved:
      return "Future already retrieved";
    case future_errc::kNoState:
      return "No state";
    case future_errc::kPromiseAlreadySatisfied:
      return "Promise already satisfied";
    default:
      return "Unknown error";
  }
}

void FutureErrorDomain::ThrowAsException(const ErrorCode& error_code) const noexcept(false) {
  throw Exception{error_code};
}
}  // namespace ara::core
