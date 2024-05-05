#ifndef VITO_AP_FUTURE_ERROR_DOMAIN_H_
#define VITO_AP_FUTURE_ERROR_DOMAIN_H_

#include <cstdint>

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara::core {
/// @brief Specifies the types of internal errors that can occur upon calling Future::get or Future::GetResult.
/// These definitions are equivalent to the ones from std::future_errc.
// ReSharper disable once CppInconsistentNaming
enum class future_errc : std::int32_t {
  kInvalidArgument,
  /// @brief the asynchronous task abandoned its shared state
  // ReSharper disable once CppInconsistentNaming
  broken_promise = 101,
  /// @brief the contents of the shared state were already accessed
  // ReSharper disable once CppInconsistentNaming
  future_already_retrieved = 102,
  /// @brief attempt to store a value into the shared state twice
  // ReSharper disable once CppInconsistentNaming
  promise_already_satisfied = 103,
  /// @brief attempt to access Promise or Future without an associated state
  // ReSharper disable once CppInconsistentNaming
  no_state = 104,
};

/// @brief Exception type thrown by Future and Promise classes.
class FutureException : public Exception {
 public:
  /// @brief Construct a new FutureException from an ErrorCode.
  /// @param err the ErrorCode
  explicit FutureException(ErrorCode err) noexcept : Exception{err} {}
};

/// @brief Error domain for errors originating from classes Future and Promise.
class FutureErrorDomain final : public ErrorDomain {
  static constexpr IdType kId{0x8000'0000'0000'0013};

 public:
  /// @brief Alias for the error code value enumeration.
  using Errc = future_errc;
  /// @brief Alias for the exception base class.
  using Exception = FutureException;

  /// @brief Default constructor
  constexpr FutureErrorDomain() noexcept : ErrorDomain{kId} {}

  /// @brief Return the "short name" ApApplicationErrorDomain.SN of this error domain.
  /// @return "Future"
  const char* Name() const noexcept override { return "Future"; }

  /// @brief Translate an error code value into a text message.
  /// @param error_code the error code value
  /// @return the text message, never nullptr
  const char* Message(CodeType error_code) const noexcept override;

  void ThrowAsException(const ErrorCode& error_code) const noexcept(false) override;
};

namespace internal {
inline constexpr FutureErrorDomain future_error_domain;
}

inline constexpr const ErrorDomain& GetFutureErrorDomain() noexcept { return internal::future_error_domain; }

inline constexpr ErrorCode MakeErrorCode(future_errc code, ErrorDomain::SupportDataType data) noexcept {
  return ErrorCode{static_cast<ErrorDomain::CodeType>(code), GetFutureErrorDomain(), data};
}
}  // namespace ara::core

#endif