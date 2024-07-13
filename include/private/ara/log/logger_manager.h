#ifndef VITO_AP_LOGGER_MANAGER_H_
#define VITO_AP_LOGGER_MANAGER_H_

#include <mutex>
#include <unordered_map>

#include "ara/core/optional.h"
#include "ara/core/string_view.h"
#include "ara/core/result.h"
#include "ara/log/common.h"
#include "ara/log/logger.h"

namespace ara::log {
class LoggerManager {
 public:
  static LoggerManager& Instance();

  core::Result<void> Init();

  Logger& CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold);

  core::Optional<std::reference_wrapper<Logger>> GetLogger(const Logger::Key& key);

 private:
  std::mutex loggers_mtx_;
  std::unordered_map<Logger::Key, std::unique_ptr<Logger>> loggers_;
};
}  // namespace ara::log

#endif