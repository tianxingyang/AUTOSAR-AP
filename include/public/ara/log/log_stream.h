#ifndef VITO_AP_LOG_STREAM_H_
#define VITO_AP_LOG_STREAM_H_

#include <chrono>
#include <memory>

#include "ara/core/error_code.h"
#include "ara/core/instance_specifier.h"
#include "ara/core/span.h"
#include "ara/core/string_view.h"
#include "ara/core/utility.h"
#include "ara/log/common.h"

namespace ara::log {
class Logger;

/// @brief Wrapper type for holding a payload argument with its attributes.
/// The setup of this class is implementation-defined.
template <typename T>
class Argument {};

class LogStream final {
 public:
  LogStream(LogLevel log_level, const Logger& logger);

  ~LogStream() noexcept;

  /// @brief Sends out the current log buffer and initiates a new message stream.
  void Flush() noexcept;

  /// @brief Appends given value to the internal message buffer.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(bool value) noexcept;

  /// @brief Writes unsigned int 8 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::uint8_t value) noexcept;

  /// @brief Writes unsigned int 16 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::uint16_t value) noexcept;

  /// @brief Writes unsigned int 32 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::uint32_t value) noexcept;

  /// @brief Writes unsigned int 64 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::uint64_t value) noexcept;

  /// @brief Writes int 8 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::int8_t value) noexcept;

  /// @brief Writes int 16 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::int16_t value) noexcept;

  /// @brief Writes int 32 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::int32_t value) noexcept;

  /// @brief Writes int 64 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(std::int64_t value) noexcept;

  /// @brief Writes float 32 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(float value) noexcept;

  /// @brief Writes float 64 bit parameter into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(double value) noexcept;

  /// @brief Writes ara::core::StringView into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(const core::StringView value) noexcept;

  /// @brief Writes null terminated UTF8 string into message.
  /// @param value Value to be appended to the internal message buffer.
  /// @return *this
  LogStream& operator<<(const char* const value) noexcept;

  /// @brief Write a byte sequence into message.
  /// This call shall copy the sequence of core::Byte objects as-is into the message.
  /// @param data a Span<const Byte> covering the range to be logged
  /// @return *this
  LogStream& operator<<(core::Span<const core::Byte> data) noexcept;

  /// @brief Set the message's privacy level.
  /// A program that calls this function with a T that is neither an integral nor an enum type is ill-formed. Only the
  /// lower 8 bits of value are used , any higher-level bits are ignored.
  /// @param value a (project-specific) value to add as privacy level of the message template
  /// @return *this
  template <typename T>
  LogStream& WithPrivacy(T value) noexcept;

  /// @brief Add source file location into the message.
  /// @param file the source file identifier
  /// @param line the source file line number
  /// @return *this
  LogStream& WithLocation(core::StringView file, int line) noexcept;

  /// @brief Add the given single tag to the current message.
  /// @param tag the tag text to attach to the current message
  /// @return *this
  LogStream& WithTag(core::StringView tag) noexcept;

 private:
  bool Enabled() const;

 private:
  struct Impl;
  std::shared_ptr<Impl> impl_;
};

// TODO
template <typename T>
LogStream& LogStream::WithPrivacy(T value) noexcept {
  return *this;
}

/// @brief Appends LogLevel enum parameter as text into message.
/// @param out LogStream Object which is used to append the logged LogLevel(value) to
/// @param value LogLevel enum parameter as text to be appended to the internal message buffer
/// @return *this
LogStream& operator<<(LogStream& out, LogLevel value) noexcept;

/// @brief Write a core::ErrorCode instance into the message.
/// When output to the console, the ErrorCode shall be shown in an implementation-defined way as a String holding the
/// result of ErrorCode:Domain().Name() (i.e. the ErrorDomain's Shortname), and the integral error code number.
/// @param out the LogStream object into which to add the value
/// @param ec the ErrorCode instance to log
/// @return out
LogStream& operator<<(LogStream& out, const core::ErrorCode& ec) noexcept;

/// @brief Write a std::chrono::duration instance into the message.
/// @tparam Rep arithmetic type representing the number of ticks in this duration
/// @tparam Period a std::ratio type representing the tick period of the clock, in seconds
/// @param out the LogStream object into which to add the value
/// @param value the duration instance to log
/// @return out
template <typename Rep, typename Period>
LogStream& operator<<(LogStream& out, const std::chrono::duration<Rep, Period>& value) noexcept {
  return out << std::chrono::duration_cast<std::chrono::duration<std::chrono::seconds>>(value).count() << "s";
}

/// @brief Write a core::InstanceSpecifier into the message.
/// @param out the LogStream object into which to add the value
/// @param value the InstanceSpecifier to log
/// @return out
LogStream& operator<<(LogStream& out, const core::InstanceSpecifier& value) noexcept;

/// @brief Write a void pointer into the message.
/// @param out the LogStream object into which to add the value
/// @param value to log
/// @return out
LogStream& operator<<(LogStream& out, const void* value) noexcept;
}  // namespace ara::log

#endif