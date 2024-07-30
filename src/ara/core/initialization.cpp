#include "ara/core/initialization.h"

#include "ara/core/result.h"
#include "ara/log/log_config.h"
#include "ara/log/logger_manager.h"

namespace ara::core {
Result<void> InitLogModule() noexcept {
  using R = Result<void>;
  if (const auto result{log::LogConfig::Instance().Init("MANIFEST.json")}; !result) {
    return R::FromError(result.Error());
  }

  if (const auto result{log::LoggerManager::Instance().Init()}; !result) {
    return R::FromError(result.Error());
  }

  return R::FromValue();
}

Result<void> Initialize() noexcept {
  using R = Result<void>;

  if (const auto result{InitLogModule()}; !result) {
    return R::FromError(result.Error());
  }

  return R::FromValue();
}

Result<void> Deinitialize() noexcept {
  using R = Result<void>;
  return R::FromValue();
}

}  // namespace ara::core
