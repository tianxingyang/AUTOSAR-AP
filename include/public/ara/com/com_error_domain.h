#ifndef VITO_AP_COM_ERROR_DOMAIN_H_
#define VITO_AP_COM_ERROR_DOMAIN_H_

#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara::com {
enum class ComErrc : core::ErrorDomain::CodeType {
  /// @brief Service is not available
  kServiceNotAvailable = 1,
  /// @brief Application holds more SamplePtrs than commited in Subscribe()
  kMaxSamplesExceeded = 2,
  /// @brief The network binding reported a recoverable communications error
  kNetworkBindingFailure = 3,
  /// @brief Request was refused by Grant enforcement layer.
  kGrantEnforcementError = 4,
  /// @brief TLS handshake fail.
  kPeerIsUnreachable = 5,
  /// @brief Field Value is not valid.
  kFieldValueIsNotValid = 6,
  /// @brief Field SetHandler has not been registered.
  kFieldSetHandlerNotSet = 7,
  /// @brief Failure detected when unregistering a handler.
  kUnsetHandlerFailure = 8,
  /// @brief Not Sufficient memory resources can be allocated.
  kSampleAllocationFailure = 9,
  /// @brief The allocation was illegally done via custom allocator (i.e., not via shared memory allocation).
  kIllegalUseOfAllocate = 10,
  /// @brief Service not offered
  kServiceNotOffered = 11,
  /// @brief ResolveInstanceIDs() failed to resolve InstanceID from Instance Specifier, i.e. is not mapped correctly
  kInstanceIDCouldNotBeResolved = 15,
  /// @brief Provided maxSampleCount not realizable.
  kMaxSampleCountNotRealizable = 16,
  /// @brief Wrong processing mode passed to constructor method call or wrong MethodCallProcessingMode for
  /// ProcessNextMethodCall().
  kWrongMethodCallProcessingMode = 17,
  /// @brief The FileHandle returned from FindService is corrupt/service not available.
  kErroneousFileHandle = 18,
  /// @brief Command Could not be executed in provided Execution Context.
  kCouldNotExecute = 19,
  /// @brief Given InstanceIdentifier string is corrupted or non-compliant.
  kInvalidInstanceIdentifierString = 20,
  /// @brief Failed to register handler.
  kSetHandlerFailure = 21,
};

class ComException : public core::Exception {
 public:
  ComException(core::ErrorCode error_code) noexcept : core::Exception(error_code) {}
};

/// @brief Defines a class representing the Communication error domain.
class ComErrorDomain final : public core::ErrorDomain {
  static constexpr IdType kId{0x8000'0000'0000'1267};

 public:
  using Errc = ComErrc;
  using Exception = ComException;

  /// @brief Constructs a new ComErrorDomain object - Not allowed.
  ComErrorDomain() = delete;

  /// @brief Returns a string constant associated with ComErrorDomain.
  /// @return "Com"
  const char* Name() const noexcept override { return "Com"; }

  /// @brief Returns the message associated with errorCode.
  /// @param error_code The error code number.
  /// @return The message associated with the error code.
  const char* Message(CodeType error_code) const noexcept override;

  void ThrowAsException(const core::ErrorCode& error_code) const noexcept(false) override;
};
}  // namespace ara::com

#endif  // VITO_AP_COM_ERROR_DOMAIN_H_