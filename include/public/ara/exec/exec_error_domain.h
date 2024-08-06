#ifndef VITO_AP_EXEC_ERROR_DOMAIN_H_
#define VITO_AP_EXEC_ERROR_DOMAIN_H_

#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara::exec {
enum class ExecErrc : core::ErrorDomain::CodeType {
  kCommunicationError = 3,  // Communication error occured.
  kMetaModelError = 4,      // Wrong meta model identifier passed to a function.
  kCanceled = 5,            // Transition to the requested Function Group state was cancelled by a newer request
  kFailed = 6,              // Requested operation could not be performed
  kFailedUnexpectedTerminationOnEnter =
      8,  // Unexpected Termination during transition in Process of target Function Group State happened
  kInvalidTransition = 9,  // Transition invalid (e.g. report kRunning when already in Running Process State)
  kNoTimestamp = 12,       // DeterministicClient time stamp information is not available
  kCycleOverrun = 13,      // DeterministicClient cycle exceeded
  kIntegrityOrAuthenticityCheckFailed =
      14,  // Integrity or authenticity check for a Process to be spawned in the requested Function Group state failed
  kFailedUnexpectedTermination =
      15,  // Unexpected Termination of a non-terminating Process during a Function Group State transition happened.
  kInvalidArgument = 16,  // Passed argument doesn't appear to be valid.
};

/// @brief Defines a class for exceptions to be thrown by the Execution Management.
class ExecException : public core::Exception {
 public:
  /// @brief Constructs a new ExecException object containing an error code.
  /// @param error_code the error code
  explicit ExecException(core::ErrorCode error_code) noexcept : core::Exception(error_code) {}
};

/// @brief Defines a class representing the Execution Management error domain.
class ExecErrorDomain final : public core::ErrorDomain {
  static constexpr IdType kId{0x8000'0000'0000'0202};

 public:
  /// @brief Constructs a new ExecErrorDomain object.
  constexpr ExecErrorDomain() noexcept : core::ErrorDomain{kId} {}

  /// @brief Returns a string constant associated with ExecErrorDomain.
  /// @return "Exec"
  const char* Name() const noexcept override { return "Exec"; }

  /// @brief Returns the message associated with errorCode.
  /// @param error_code the error code number
  /// @return The message associated with the error code.
  const char* Message(CodeType error_code) const noexcept override;

  /// @brief Creates a new instance of ExecException from errorCode and throws it as a C++ exception.
  /// @param error_code The error to throw.
  void ThrowAsException(const core::ErrorCode& error_code) const noexcept(false) override;
};

namespace internal {
inline constexpr ExecErrorDomain exec_error_domain{};
}  // namespace internal

constexpr const core::ErrorDomain& GetExecErrorDomain() noexcept { return internal::exec_error_domain; }

constexpr core::ErrorCode MakeErrorCode(ExecErrc code, core::ErrorDomain::SupportDataType data) noexcept {
  return core::ErrorCode{static_cast<core::ErrorDomain::CodeType>(code), GetExecErrorDomain(), data};
}
}  // namespace ara::exec

#endif