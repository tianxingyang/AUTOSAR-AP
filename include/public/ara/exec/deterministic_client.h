#ifndef VITO_AP_DETERMINISTIC_CLIENT_H_
#define VITO_AP_DETERMINISTIC_CLIENT_H_

#include <chrono>
#include <cstdint>

namespace ara::exec {
enum class ActivationReturnType : std::uint32_t {
  kRegisterServices = 0,
  kServiceDiscovery = 1,
  kInit = 2,
  kRun = 3,
  kTerminate = 4,
};

using ara
}  // namespace ara::exec

#endif  // VITO_AP_DETERMINISTIC_CLIENT_H_