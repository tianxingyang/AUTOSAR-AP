#include "ara/log/logger.h"

#include "ara/log/logger_manager.h"
#include "fmt/format.h"

namespace {
constexpr std::uint16_t kDefaultIntPrecision{7};
}

namespace ara::log {
constexpr Format Dflt() noexcept { return Format{Fmt::kDefault}; }

constexpr Format Dec() noexcept { return Dec(kDefaultIntPrecision); }

constexpr Format Dec(std::uint16_t precision) noexcept { return Format{Fmt::kDec, precision}; }

constexpr Format Oct() noexcept { return Oct(kDefaultIntPrecision); }

constexpr Format Oct(std::uint16_t precision) noexcept { return Format{Fmt::kOct, precision}; }

constexpr Format Hex() noexcept { return Hex(kDefaultIntPrecision); };

constexpr Format Hex(std::uint16_t precision) noexcept { return Format{Fmt::kHex, precision}; };

constexpr Format Bin() noexcept { return Bin(kDefaultIntPrecision); }

constexpr Format Bin(std::uint16_t precision) noexcept { return Format{Fmt::kBin, precision}; }

constexpr Format DecFloat(std::uint16_t precision) noexcept { return Format{Fmt::kDecFloat, precision}; }

constexpr Format DecFloatMax() noexcept { return DecFloat(UINT16_MAX); }

constexpr Format EngFloat(std::uint16_t precision) noexcept { return Format{Fmt::kEngFloat, precision}; }

constexpr Format EngFloatMax() noexcept { return EngFloat(UINT16_MAX); }

constexpr Format HexFloat(std::uint16_t precision) noexcept { return Format{Fmt::kHexFloat, precision}; }

constexpr Format HexFloatMax() noexcept { return HexFloat(UINT16_MAX); }

constexpr Format AutoFloat(std::uint16_t precision) noexcept { return Format{Fmt::kAutoFloat, precision}; }

constexpr Format AutoFloatMax() noexcept { return AutoFloat(UINT16_MAX); }

struct Logger::Impl {
  core::String ctx_id;
  core::String ctx_desc;
  LogLevel threshold;
};

void Logger::SetThreshold(LogLevel threshold) { impl_->threshold = threshold; }

Logger::Logger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold)
    : impl_{std::make_shared<Impl>()} {
  impl_->ctx_id = ctx_id;
  impl_->ctx_desc = ctx_desc;
  impl_->threshold = threshold;
}

const Logger::Key& Logger::GetKey() const { return impl_->ctx_id; }

void Logger::Handle(const LogStream& log_stream) {
  auto handlers_result = LoggerManager::Instance().GetLoggingHandlers(GetKey());
  if (!handlers_result) {
    fmt::print("get handlers failed, {}", handlers_result.Error().Message());
    return;
  }

  for (auto& handler : handlers_result->get()) {
    handler->Emit(log_stream);
  }
}

Logger& CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel ctx_def_log_level) {
  return LoggerManager::Instance().CreateLogger(ctx_id, ctx_desc, ctx_def_log_level);
}

LogStream Logger::LogFatal() const noexcept { return {LogLevel::kFatal, *this}; }

LogStream Logger::LogError() const noexcept { return {LogLevel::kError, *this}; }

LogStream Logger::LogWarn() const noexcept { return {LogLevel::kWarn, *this}; }

LogStream Logger::LogInfo() const noexcept { return {LogLevel::kInfo, *this}; }

LogStream Logger::LogDebug() const noexcept { return {LogLevel::kDebug, *this}; }

LogStream Logger::LogVerbose() const noexcept { return {LogLevel::kVerbose, *this}; }

bool Logger::IsEnabled(LogLevel log_level) const noexcept { return log_level <= impl_->threshold; }

LogStream Logger::WithLevel(LogLevel log_level) const noexcept { return {log_level, *this}; }
}  // namespace ara::log