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
    log_sinks_ = config["LogSinks"].get<core::Vector<core::String>>();
    app_id_ = config["AppId"].get<core::String>();
    return R::FromValue();
  } catch (...) {
    return R::FromError(LogErrc::kInvalidConfig);
  }
}

const core::String& LogConfig::EcuId() const { return ecu_id_; }

const core::Vector<core::String>& LogConfig::LogSinks() const { return log_sinks_; }

const core::String& LogConfig::AppId() const { return app_id_; }
}  // namespace ara::log
