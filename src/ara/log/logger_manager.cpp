#include "ara/log/logger_manager.h"

namespace ara::log {

LoggerManager& LoggerManager::Instance() {
  static LoggerManager singleton;
  return singleton;
}

core::Result<void> LoggerManager::Init() { return {}; }

Logger& LoggerManager::CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold) {
  auto const logger = new Logger(ctx_id, ctx_desc, threshold);
  auto& key = logger->GetKey();
  std::scoped_lock lock{loggers_mtx_};
  return *loggers_.emplace(key, std::unique_ptr<Logger>(logger)).first->second;
}

core::Optional<std::reference_wrapper<Logger>> LoggerManager::GetLogger(const Logger::Key& key) {
  if (const auto it = loggers_.find(key); it == loggers_.end()) {
    return std::nullopt;
  } else {
    return {*it->second};
  }
}
}  // namespace ara::log
