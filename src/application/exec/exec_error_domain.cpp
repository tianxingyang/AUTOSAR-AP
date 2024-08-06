#include "ara/exec/exec_error_domain.h"

namespace ara::exec {
const char* ExecErrorDomain::Message(CodeType error_code) const noexcept {
  switch (const auto code{static_cast<ExecErrc>(error_code)}) {
    case ExecErrc::kCommunicationError:
      return "Communication error occured.";
    case ExecErrc::kMetaModelError:
      return "Wrong meta model identifier passed to a function.";
    case ExecErrc::kCanceled:
      return "Transition to the requested Function Group state was cancelled by a newer request";
    case ExecErrc::kFailed:
      return "Requested operation could not be performed";
    case ExecErrc::kFailedUnexpectedTerminationOnEnter:
      return "Unexpected Termination during transition in Process of target Function Group State happened";
    case ExecErrc::kInvalidTransition:
      return "Transition invalid (e.g. report kRunning when already in Running Process State)";
    case ExecErrc::kNoTimestamp:
      return "DeterministicClient time stamp information is not available";
    case ExecErrc::kCycleOverrun:
      return "DeterministicClient cycle exceeded";
    case ExecErrc::kIntegrityOrAuthenticityCheckFailed:
      return "Integrity or authenticity check for a Process to be spawned in the requested Function Group state failed";
    case ExecErrc::kFailedUnexpectedTermination:
      return "Unexpected Termination of a non-terminating Process during a Function Group State transition happened.";
    case ExecErrc::kInvalidArgument:
      return "Passed argument doesn't appear to be valid.";
    default:
      return "Unknown error";
  }
}

void ExecErrorDomain::ThrowAsException(const core::ErrorCode& error_code) const noexcept(false) {
  throw ExecException{error_code};
}
}  // namespace ara::exec