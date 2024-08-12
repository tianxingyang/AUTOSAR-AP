#include "ara/com/com_error_domain.h"

namespace ara::com {
const char* ComErrorDomain::Message(CodeType error_code) const noexcept {
  switch (static_cast<ComErrc>(error_code)) {
    case ComErrc::kServiceNotAvailable:
      return "Service not available";
    case ComErrc::kMaxSamplesExceeded:
      return "Max samples exceeded";
    case ComErrc::kNetworkBindingFailure:
      return "Network binding failure";
    case ComErrc::kGrantEnforcementError:
      return "Grant enforcement error";
    case ComErrc::kPeerIsUnreachable:
      return "Peer is unreachable";
    case ComErrc::kFieldValueIsNotValid:
      return "Field value is not valid";
    case ComErrc::kFieldSetHandlerNotSet:
      return "Field set handler not set";
    case ComErrc::kUnsetHandlerFailure:
      return "Unset handler failure";
    case ComErrc::kSampleAllocationFailure:
      return "Sample allocation failure";
    case ComErrc::kIllegalUseOfAllocate:
      return "Illegal use of allocate";
    case ComErrc::kServiceNotOffered:
      return "Service not offered";
    case ComErrc::kInstanceIDCouldNotBeResolved:
      return "Instance ID could not be resolved";
    case ComErrc::kMaxSampleCountNotRealizable:
      return "Max sample count not realizable";
    case ComErrc::kWrongMethodCallProcessingMode:
      return "Wrong method call processing mode";
    case ComErrc::kErroneousFileHandle:
      return "Erroneous file handle";
    case ComErrc::kCouldNotExecute:
      return "Could not execute";
    case ComErrc::kInvalidInstanceIdentifierString:
      return "Invalid instance identifier string";
    case ComErrc::kSetHandlerFailure:
      return "Set handler failure";
    default:
      return "Unknown error";
  }
}
}  // namespace ara::com