#include "ara/exec/execution_client.h"

#include "ara/exec/exec_error_domain.h"

namespace ara::exec {
ExecutionClient::ExecutionClient(TerminationHandler termination_handler) {
  if (!termination_handler) {
    GetExecErrorDomain().ThrowAsException(ExecErrc::kInvalidArgument);
  }
}

core::Result<ExecutionClient> ExecutionClient::Create(TerminationHandler termination_handler) noexcept {
  using R = core::Result<ExecutionClient>;
  try {
    return R{ExecutionClient{std::move(termination_handler)}};
  } catch (const core::Exception& e) {
    return R{e.Error()};
  }
}

core::Result<void> ExecutionClient::ReportExecutionState(ExecutionState state) const noexcept { return {}; }
}  // namespace ara::exec