#ifndef VITO_AP_LOG_ERROR_DOMAIN_H_
#define VITO_AP_LOG_ERROR_DOMAIN_H_

#include <cstdint>

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara::log {
enum class LogErrc : std::uint8_t {
  kBufferOverflow = 0,
  kLoggerNotFound = 1,
  kInvalidConfig = 2,
};

class LogException : public core::Exception {
 public:
  /// @brief Construct a new FutureException from an ErrorCode.
  /// @param err the ErrorCode
  explicit LogException(core::ErrorCode err) noexcept : Exception{err} {}
};

class LogErrorDomain final : public core::ErrorDomain {
  static constexpr IdType kId{0x8100'0000'0000'0001};

 public:
  /// @brief Alias for the error code value enumeration.
  using Errc = LogErrc;
  /// @brief Alias for the exception base class.
  using Exception = LogException;

  /// @brief Default constructor
  constexpr LogErrorDomain() noexcept : ErrorDomain{kId} {}

  /// @brief Return the "short name" ApApplicationErrorDomain.SN of this error domain.
  /// @return "Log"
  const char* Name() const noexcept override { return "Log"; }

  /// @brief Translate an error code value into a text message.
  /// @param error_code the error code value
  /// @return the text message, never nullptr
  const char* Message(CodeType error_code) const noexcept override;

  void ThrowAsException(const core::ErrorCode& error_code) const noexcept(false) override;
};
namespace internal {
inline constexpr LogErrorDomain log_error_domain;
}

inline constexpr const core::ErrorDomain& GetLogErrorDomain() noexcept { return internal::log_error_domain; }

inline constexpr core::ErrorCode MakeErrorCode(LogErrc code, core::ErrorDomain::SupportDataType data) noexcept {
  return core::ErrorCode{static_cast<core::ErrorDomain::CodeType>(code), GetLogErrorDomain(), data};
}

}  // namespace ara::log

#endif