#ifndef VITO_AP_EXECUTION_CLIENT_H_
#define VITO_AP_EXECUTION_CLIENT_H_

#include <cstdint>

#include "ara/core/result.h"

namespace ara::exec {
/// @brief Defines the internal states of Process. Scoped Enumeration of uint8_t.
enum class ExecutionState : std::uint8_t {
  kRunning = 0,  // After a Process has been started by Execution Management, it reports ExecutionState kRunning.
};

class ExecutionClient {
 public:
  /// @brief Destructor.
  ~ExecutionClient() noexcept = default;

  /// @brief Returns the current state of the Process.
  /// @return The current state of the Process.
  ara::core::Result<void> ReportExecutionState(ExecutionState state) const noexcept;
};
}  // namespace ara::exec

#endif