#include "ara/log/log_config.h"

#include <fstream>

#include "ara/log/log_error_domain.h"
#include "nlohmann/json.hpp"

namespace ara::log {

core::Result<void> LogConfig::Init(core::StringView config_path) {
  using R = core::Result<void>;

  try {
    nlohmann::json config;
    std::ifstream file(config_path.data());
    file >> config;
    ecu_id_ = config["EcuId"].get<core::String>();
    return R::FromValue();
  } catch (...) {
    return R::FromError(LogErrc::kInvalidConfig);
  }
}

const core::String& LogConfig::EcuId() const { return ecu_id_; }
}  // namespace ara::log
