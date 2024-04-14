#include "ara/core/initialization.h"

namespace ara::core {
Result<void> Initialize() noexcept {
  using R = Result<void>;
  return R::FromValue();
}

Result<void> Deinitialize() noexcept {
  using R = Result<void>;
  return R::FromValue();
}

}  // namespace ara::core
