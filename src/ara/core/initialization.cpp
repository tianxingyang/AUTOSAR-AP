#include "ara/core/initialization.h"

#include "ara/core/result.h"
#include "ara/log/log_config.h"

namespace ara::core {
Result<void> Initialize() noexcept {
  using R = Result<void>;

  if (const auto result{log::LogConfig::Instance().Init("config.json")}; !result) {
    return R::FromError(result.Error());
  }

  return R::FromValue();
}

Result<void> Deinitialize() noexcept {
  using R = Result<void>;
  return R::FromValue();
}

}  // namespace ara::core
