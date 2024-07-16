#include "ara/log/logger_manager.h"

#include "ara/core/result.h"
#include "ara/log/common.h"
#include "ara/log/log_error_domain.h"

namespace ara::log {

LoggerManager& LoggerManager::Instance() {
  static LoggerManager singleton;
  return singleton;
}

core::Result<void> LoggerManager::Init() { return {}; }

Logger& LoggerManager::CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold) {
  const auto logger = new Logger(ctx_id, ctx_desc, threshold);
  auto& key = logger->GetKey();
  std::scoped_lock lock{loggers_mtx_};
  return *loggers_.emplace(key, std::make_unique<internal::LoggerImpl>(logger)).first->second->logger;
}

core::Optional<std::reference_wrapper<internal::LoggerImpl>> LoggerManager::GetLoggerImpl(const Logger::Key& key) {
  std::scoped_lock lock{loggers_mtx_};
  if (const auto it = loggers_.find(key); it == loggers_.end()) {
    return std::nullopt;
  } else {
    return {*it->second};
  }
}

core::Optional<std::reference_wrapper<Logger>> LoggerManager::GetLogger(const Logger::Key& key) {
  std::scoped_lock lock{loggers_mtx_};
  if (const auto it = loggers_.find(key); it == loggers_.end()) {
    return std::nullopt;
  } else {
    return {*it->second->logger};
  }
}

core::Result<void> LoggerManager::AddLoggingHandler(Logger::Key key, std::unique_ptr<LoggingHandler> logging_handler) {
  using R = core::Result<void>;
  std::scoped_lock lock{loggers_mtx_};
  auto logger_impl_opt = GetLoggerImpl(key);
  if (!logger_impl_opt) {
    return R::FromError(LogErrc::kLoggerNotFound);
  }

  auto& logger_impl = logger_impl_opt->get();
  logger_impl.logging_handlers.emplace_back(std::move(logging_handler));
  return {};
}

core::Result<std::reference_wrapper<core::Vector<std::unique_ptr<LoggingHandler>>>> LoggerManager::GetLoggingHandlers(Logger::Key key) {
  using R = decltype(GetLoggingHandlers(key));
  auto logger_impl_opt = GetLoggerImpl(key);
  if (!logger_impl_opt) {
    return R::FromError(LogErrc::kLoggerNotFound);
  }

  return R::FromValue(logger_impl_opt->get().logging_handlers);
}
}  // namespace ara::log
