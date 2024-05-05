#include "ara/log/log_stream.h"

#include <cstdint>

#include "ara/core/utility.h"
#include "ara/log/dlt_message.h"
#include "ara/log/log_stream_buffer.h"
#include "fmt/format.h"

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
  Buffer buffer;
};

void LogStream::Flush() noexcept {}

LogStream& LogStream::operator<<(bool value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::uint8_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::uint16_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::uint32_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::uint64_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::int8_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::int16_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& ara::log::LogStream::operator<<(std::int32_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(std::int64_t value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(float value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(double value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(const core::StringView value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(const char* const value) noexcept {
  impl_->buffer.Append(value);
  return *this;
}

LogStream& LogStream::operator<<(core::Span<const core::Byte> data) noexcept {
  impl_->buffer.Append(data);
  return *this;
}

LogStream& LogStream::WithLocation(core::StringView file, int line) noexcept { return *this; }

LogStream& LogStream::WithTag(core::StringView tag) noexcept { return *this; }

LogStream& operator<<(LogStream& out, LogLevel value) noexcept { return out << LogLevel2String(value); }

LogStream& operator<<(LogStream& out, const core::InstanceSpecifier& value) noexcept { return out << value.ToString(); }

LogStream& operator<<(LogStream& out, const void* value) noexcept { return out << fmt::format("{}", value); }
}  // namespace ara::log