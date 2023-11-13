#ifndef VITO_AP_CORE_ERROR_DOMAIN_H_
#define VITO_AP_CORE_ERROR_DOMAIN_H_

#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara::core {
/// @brief An enumeration that defines all errors of the CORE Functional Cluster.
enum class CoreErrc : ErrorDomain::CodeType {
  /// @brief an invalid argument was passed to a function
  kInvalidArgument = 22,
  /// @brief given string is not a valid model element short name
  kInvalidMetaModelShortName = 137,
  /// @brief missing or invalid path to model element
  kInvalidMetaModelPath = 138,
};

/// @brief Exception type thrown for CORE errors.
class CoreException final : public Exception {
 public:
  /// @brief Construct a new CoreException from an ErrorCode.
  /// @param err the ErrorCode
  explicit CoreException(ErrorCode err) noexcept : Exception{err} {}
};

/// @brief An error domain for errors originating from the CORE Functional Cluster.
class CoreErrorDomain final : public ErrorDomain {
  static constexpr IdType kId{0x8000'0000'0000'0014};

 public:
  /// @brief Alias for the error code value enumeration.
  using Errc = CoreErrc;
  /// @brief Alias for the exception base class.
  using Exception = CoreException;

  /// @brief Default constructor.
  constexpr CoreErrorDomain() noexcept : ErrorDomain{kId} {}

  /// @brief Return the "short name" ApApplicationErrorDomain.SN of this error domain.
  /// @return "Core"
  const char* Name() const noexcept override { return "Core"; }

  /// @brief Translate an error code value into a text message.
  /// @param error_code the error code value
  /// @return the text message, never nullptr
  const char* Message(CodeType error_code) const noexcept override;

  /// @brief Throw the exception type corresponding to the given ErrorCode.
  /// @param error_code the ErrorCode instance
  void ThrowAsException(const ErrorCode& error_code) const noexcept(false) override;
};

namespace internal {
inline constexpr CoreErrorDomain core_error_domain_ins;
}

/// @brief Return a reference to the global CoreErrorDomain.
/// @return the CoreErrorDomain
constexpr const ErrorDomain& GetCoreErrorDomain() noexcept { return internal::core_error_domain_ins; }

/// @brief Create a new ErrorCode within CoreErrorDomain.
/// This function is used internally by constructors of ErrorCode. It is usually not used directly by users.
/// @param code the CoreErrorDomain-specific error code value
/// @param data optional vendor-specific error data
/// @return a new ErrorCode instance
constexpr ErrorCode MakeErrorCode(CoreErrc code, ErrorDomain::SupportDataType data) noexcept {
  return ErrorCode{static_cast<ErrorDomain::CodeType>(code), GetCoreErrorDomain(), data};
}
}  // namespace ara::core

#endif