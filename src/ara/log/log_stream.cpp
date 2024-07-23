#include "ara/log/log_stream.h"

#include <fmt/format.h>

#include <cstdint>

#include "ara/core/utility.h"
#include "ara/log/dlt_message.h"
#include "ara/log/logger.h"
#include "ara/log/logger_manager.h"
#include "fmt/format.h"
#include "fmt/ranges.h"

namespace {
ara::core::StringView LogLevel2String(ara::log::LogLevel log_level) {
  switch (log_level) {
    case ara::log::LogLevel::kOff:
      return "Off";
    case ara::log::LogLevel::kFatal:
      return "Fatal";
    case ara::log::LogLevel::kError:
      return "Error";
    case ara::log::LogLevel::kWarn:
      return "Warn";
    case ara::log::LogLevel::kInfo:
      return "Info";
    case ara::log::LogLevel::kDebug:
      return "Debug";
    case ara::log::LogLevel::kVerbose:
      return "Verbose";
    default:
      return "Unknown";
  }
}
}  // namespace

namespace ara::log {
struct LogStream::Impl {
  LogLevel log_level;
  Logger::Key owner_key;
  std::shared_ptr<dlt::Message> dlt_message{nullptr};
};

LogStream::LogStream(LogLevel log_level, const Logger& logger) : impl_{std::make_shared<Impl>()} {
  impl_->dlt_message = dlt::Message::VerboseModeLogMessage(log_level);
  impl_->log_level = log_level;
}

void LogStream::Flush() noexcept {
  auto logger_opt = LoggerManager::Instance().GetLogger(impl_->owner_key);
  if (!logger_opt) {
    fmt::print("logger[{}] doesn't exist.", impl_->owner_key);
    return;
  }

  logger_opt->get().Handle(impl_->dlt_message);
}

LogStream& LogStream::operator<<(bool value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::uint8_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::uint16_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::uint32_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::uint64_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::int8_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::int16_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& ara::log::LogStream::operator<<(std::int32_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(std::int64_t value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(float value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(double value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(const core::StringView value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value.data());
  }
  return *this;
}

LogStream& LogStream::operator<<(const char* const value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(value);
  }
  return *this;
}

LogStream& LogStream::operator<<(core::Span<const core::Byte> value) noexcept {
  if (Enabled()) {
    impl_->dlt_message->AddArgument(fmt::format("{}", fmt::join(value, "")).data());
  }
  return *this;
}

LogStream& LogStream::WithLocation(core::StringView file, int line) noexcept { return *this; }

LogStream& LogStream::WithTag(core::StringView tag) noexcept { return *this; }

bool LogStream::Enabled() const {
  const auto loggerOpt = LoggerManager::Instance().GetLogger(impl_->owner_key);
  if (!loggerOpt) {
    return false;
  }
  return loggerOpt->get().IsEnabled(impl_->log_level);
}

LogStream& operator<<(LogStream& out, LogLevel value) noexcept { return out << LogLevel2String(value); }

LogStream& operator<<(LogStream& out, const core::InstanceSpecifier& value) noexcept { return out << value.ToString(); }

LogStream& operator<<(LogStream& out, const void* value) noexcept { return out << fmt::format("{}", value); }
}  // namespace ara::log