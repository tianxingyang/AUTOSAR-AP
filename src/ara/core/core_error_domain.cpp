#include "ara/core/core_error_domain.h"

namespace ara::core {
const char* CoreErrorDomain::Message(CodeType error_code) const noexcept {
  switch (const auto code{static_cast<Errc>(error_code)}) {
    case CoreErrc::kInvalidArgument:
      return "Invalid argument";
    case CoreErrc::kInvalidMetaModelShortName:
      return "Invalid meta model short name";
    case CoreErrc::kInvalidMetaModelPath:
      return "Invalid meta model path";
    default:
      return "Unknown error";
  }
}

void CoreErrorDomain::ThrowAsException(const ErrorCode& error_code) const noexcept(false) {
  throw Exception{error_code};
}
}  // namespace ara::core
