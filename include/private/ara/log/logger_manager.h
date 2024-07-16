#ifndef VITO_AP_LOGGER_MANAGER_H_
#define VITO_AP_LOGGER_MANAGER_H_

#include <mutex>
#include <unordered_map>

#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/log/common.h"
#include "ara/log/logger.h"
#include "ara/log/logging_handler.h"

namespace ara::log {
namespace internal {
struct LoggerImpl {
  explicit LoggerImpl(Logger* logger_ptr) : logger{std::unique_ptr<Logger>(logger_ptr)} {}
  std::unique_ptr<Logger> logger;
  core::Vector<std::unique_ptr<LoggingHandler>> logging_handlers;
};
}  // namespace internal

class LoggerManager {
 public:
  static LoggerManager& Instance();

  core::Result<void> Init();

  Logger& CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold);

  core::Optional<std::reference_wrapper<Logger>> GetLogger(const Logger::Key& key);

  core::Result<void> AddLoggingHandler(Logger::Key key, std::unique_ptr<LoggingHandler> logging_handler);

  core::Result<std::reference_wrapper<core::Vector<std::unique_ptr<LoggingHandler>>>> GetLoggingHandlers(Logger::Key key);

 private:
  core::Optional<std::reference_wrapper<internal::LoggerImpl>> GetLoggerImpl(const Logger::Key& key);

 private:
  std::mutex loggers_mtx_;
  std::unordered_map<Logger::Key, std::unique_ptr<internal::LoggerImpl>> loggers_;
};
}  // namespace ara::log

#endif