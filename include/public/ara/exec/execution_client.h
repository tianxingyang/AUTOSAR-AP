#ifndef VITO_AP_EXECUTION_CLIENT_H_
#define VITO_AP_EXECUTION_CLIENT_H_

#include <cstdint>
#include <functional>

#include "ara/core/result.h"

namespace ara::exec {
/// @brief Defines the internal states of Process. Scoped Enumeration of uint8_t.
enum class ExecutionState : std::uint8_t {
  kRunning = 0,  // After a Process has been started by Execution Management, it reports ExecutionState kRunning.
};

/// @brief Class to implement operations on Execution Client.
class ExecutionClient {
 public:
  using TerminationHandler = std::function<void()>;
  /// @brief Regular constructor for ExecutionClient.
  /// @param termination_handler Callback which is called if ExecutionClient receives SIGTERM signal.The callback is
  /// executed in a background thread. A typical implementation of this callback will set a global flag(and potentially
  /// unblock other threads) to perform a graceful termination.The lifetime of the given function has to exceed the
  /// lifetime of the ExecutionClient object.
  ExecutionClient(TerminationHandler termination_handler);

  /// @brief noexcept destructor
  ~ExecutionClient() noexcept = default;

  /// @brief Suppress default copy construction for ExecutionClient.
  ExecutionClient(const ExecutionClient&) = delete;

  /// @brief Suppress default copy assignment for ExecutionClient.
  ExecutionClient& operator=(const ExecutionClient&) = delete;

  /// @brief
  /// @param
  ExecutionClient(ExecutionClient&&) noexcept = default;

  /// @brief 
  /// @param  
  /// @return 
  ExecutionClient& operator=(ExecutionClient&&) noexcept = default;

  /// @brief Named constructor for ExecutionClient.
  /// @param termination_handler Callback which is called if ExecutionClient receives SIGTERM signal.The callback is
  /// executed in a background thread. A typical implementation of this callback will set a global flag(and potentially
  /// unblock other threads) to perform a graceful termination.The lifetime of the given function has to exceed the
  /// lifetime of the ExecutionClient object.
  /// @return a result that contains either a ExecutionClient object or an error.
  static core::Result<ExecutionClient> Create(TerminationHandler termination_handler) noexcept;

  /// @brief Returns the current state of the Process.
  /// @return The current state of the Process.
  core::Result<void> ReportExecutionState(ExecutionState state) const noexcept;
};
}  // namespace ara::exec

#endif