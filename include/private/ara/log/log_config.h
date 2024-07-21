#ifndef VITO_AP_LOG_CONFIG_H_
#define VITO_AP_LOG_CONFIG_H_

#include "ara/core/result.h"
#include "ara/core/singleton_pattern.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"

namespace ara::log {
class LogConfig : public core::Singleton<LogConfig> {
 public:
  core::Result<void> Init(core::StringView config_path);

  const core::String& EcuId() const;

 private:
  core::String ecu_id_;
};
}  // namespace ara::log

#endif