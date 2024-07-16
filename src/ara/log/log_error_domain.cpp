#include "ara/log/log_error_domain.h"

namespace ara::log {
const char* LogErrorDomain::Message(CodeType error_code) const noexcept {
  switch (const auto code{static_cast<Errc>(error_code)}) {
    case Errc::kBufferOverflow:
      return "buffer overflow";
    case Errc::kLoggerNotFound:
      return "logger not found";
    default:
      return "Unknown error";
  }
}

void ara::log::LogErrorDomain::ThrowAsException(const core::ErrorCode& error_code) const noexcept(false) {
  throw Exception{error_code};
}
}  // namespace ara::log