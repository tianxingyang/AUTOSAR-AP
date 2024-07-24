#include "ara/log/logger_manager.h"

#include "ara/core/result.h"
#include "ara/log/common.h"
#include "ara/log/log_config.h"
#include "ara/log/log_error_domain.h"
#include "ara/log/logging_handler.h"

namespace ara::log {
core::Result<void> LoggerManager::Init() {
  using R = core::Result<void>;
  const auto log_sinks = LogConfig::Instance().LogSinks();
  for (const auto& log_sink : log_sinks) {
    if (log_sink == "CONSOLE") {
      logging_handlers_.emplace_back(std::make_unique<ConsoleHandler>());
    } else {
      return R::FromError(LogErrc::kInvalidLogSink);
    }
  }

  return {};
}

Logger& LoggerManager::CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold) {
  const auto logger = new Logger(ctx_id, ctx_desc, threshold);
  auto& key = logger->GetKey();
  std::scoped_lock lock{loggers_mtx_};
  return *loggers_.emplace(key, std::unique_ptr<Logger>(logger)).first->second;
}

core::Optional<std::reference_wrapper<Logger>> LoggerManager::GetLogger(const Logger::Key& key) {
  std::scoped_lock lock{loggers_mtx_};
  if (const auto it = loggers_.find(key); it == loggers_.end()) {
    return std::nullopt;
  } else {
    return {*it->second};
  }
}

const core::Vector<std::unique_ptr<LoggingHandler>>& LoggerManager::GetLoggingHandlers() { return logging_handlers_; }
}  // namespace ara::log
