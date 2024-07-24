#ifndef VITO_AP_LOGGER_H_
#define VITO_AP_LOGGER_H_

#include <cstdint>

#include "ara/core/string.h"
#include "ara/log/log_stream.h"

namespace ara::log {

namespace dlt {
class Message;
}

/// @brief Format specifiers for log message arguments.
enum class Fmt : std::uint16_t {
  /// @brief implementation-defined formatting
  kDefault = 0,
  /// @brief decimal(signed/unsigned)
  kDec = 1,
  /// @brief octal
  kOct = 2,
  /// @brief hexadecimal
  kHex = 3,
  /// @brief binary
  kBin = 4,
  /// @brief decimal float(like printf "%f")
  kDecFloat = 5,
  /// @brief engineering float(like printf "%e")
  kEngFloat = 6,
  /// @brief hex float(like printf "%a")
  kHexFloat = 7,
  /// @brief automatic "shortest" float(like printf "%g")
  kAutoFloat = 8,
};

/// @brief A type holding a formatting hint.
/// The interpretation of precision depends on fmt:
/// For integral types(i.e.Fmt::kDec, Fmt::kOct, Fmt::kHex, Fmt::kBin), precision is interpreted as the minimum number
/// of digits to output, similar to e.g.std::printf("%.7d").
/// For the floating - point specifiers Fmt::kDecFloat, Fmt::kEngFloat and Fmt::kHexFloat, precision denotes the exact
/// number of digits to be shown after the decimal point; for Fmt::kAutoFloat, precision denotes the number of
/// significant digits to be shown according to the rules of the std::printf "%g" specifier.
/// If fmt is Fmt::kDefault, the precision field is ignored, and an implementation-defined formatting is applied.For
/// integral types, if precision is 0, it is interpreted the same as if it was 1.
struct Format {
  /// @brief the format specifier
  Fmt fmt;
  /// @brief the precision to use
  std::uint16_t precision;
};

/// @brief Create a Format instance with Fmt::kDefault formatting hint.
/// @return a Format instance
constexpr Format Dflt() noexcept;

/// @brief Create a Format instance with Fmt::kDec formatting hint and default precision.
/// @return a Format instance
constexpr Format Dec() noexcept;

/// @brief Create a Format instance with Fmt::kDec formatting hint and given precision.
/// @param precision
/// @return a Format instance
constexpr Format Dec(std::uint16_t precision) noexcept;

/// @brief Create a Format instance with Fmt::kOct formatting hint and default precision.
/// @return a Format instance
constexpr Format Oct() noexcept;

/// @brief Create a Format instance with Fmt::kOct formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format Oct(std::uint16_t precision) noexcept;

/// @brief Create a Format instance with Fmt::kHex formatting hint and default precision.
/// @return a Format instance
constexpr Format Hex() noexcept;

/// @brief Create a Format instance with Fmt::kHex formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format Hex(std::uint16_t precision) noexcept;

/// @brief Create a Format instance with Fmt::kBin formatting hint and default precision.
/// @return a Format instance
constexpr Format Bin() noexcept;

/// @brief Create a Format instance with Fmt::kBin formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format Bin(std::uint16_t precision) noexcept;

/// @brief Create a Format instance with Fmt::kDecFloat formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format DecFloat(std::uint16_t precision = 6) noexcept;

/// @brief Create a Format instance with Fmt::kDecFloat formatting hint and a precision that is sufficient for full
/// round-trip safety.
/// @return a Format instance
constexpr Format DecFloatMax() noexcept;

/// @brief Create a Format instance with Fmt::kEngFloat formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format EngFloat(std::uint16_t precision = 6) noexcept;

/// @brief Create a Format instance with Fmt::kEngFloat formatting hint and a precision that is sufficient for full
/// round-trip safety.
/// @return a Format instance
constexpr Format EngFloatMax() noexcept;

/// @brief Create a Format instance with Fmt::kHexFloat formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format HexFloat(std::uint16_t precision) noexcept;

/// @brief Create a Format instance with Fmt::kHexFloat formatting hint and a precision that is sufficient for full
/// round-trip safety.
/// @return a Format instance
constexpr Format HexFloatMax() noexcept;

/// @brief Create a Format instance with Fmt::kAutoFloat formatting hint and given precision.
/// @param precision the precision to use
/// @return a Format instance
constexpr Format AutoFloat(std::uint16_t precision = 6) noexcept;

/// @brief Create a Format instance with Fmt::kAutoFloat formatting hint and a precision that is sufficient for full
/// round-trip safety.
/// @return a Format instance
constexpr Format AutoFloatMax() noexcept;

/// @brief Interface for sending log messages.
class Logger {
 public:
  using Key = core::String;
  /// @brief Creates a LogStream object.
  /// Returned object will accept arguments via the insert stream operator "@c <<".
  /// @return LogStream object of Fatal severity.
  [[nodiscard]] LogStream LogFatal() const noexcept;

  /// @brief Same as Logger::LogFatal().
  /// @return LogStream object of Error severity.
  [[nodiscard]] LogStream LogError() const noexcept;

  /// @brief Same as Logger::LogFatal().
  /// @return LogStream object of Warn severity.
  [[nodiscard]] LogStream LogWarn() const noexcept;

  /// @brief Same as Logger::LogFatal().
  /// @return LogStream object of Info severity.
  [[nodiscard]] LogStream LogInfo() const noexcept;

  /// @brief Same as Logger::LogFatal().
  /// @return LogStream object of Debug severity.
  [[nodiscard]] LogStream LogDebug() const noexcept;

  /// @brief Same as Logger::LogFatal().
  /// @return LogStream object of Verbose severity.
  [[nodiscard]] LogStream LogVerbose() const noexcept;

  /// @brief Check current configured log reporting level.
  /// Applications may want to check the actual configured reporting log level of certain loggers before doing log data
  /// preparation that is runtime intensive.
  /// @param log_level The to be checked log level.
  /// @return True if desired log level satisfies the configured reporting level.
  [[nodiscard]] bool IsEnabled(LogLevel log_level) const noexcept;

  /// @brief Log message with a programmatically determined log level can be written.
  /// @param log_level the log level to use for this LogStream instance
  /// @return a new LogStream instance with the given log level
  [[nodiscard]] LogStream WithLevel(LogLevel log_level) const noexcept;

  /// @brief Log a modeled message.
  /// If this function is called with an argument list that does not match the modeled message, the program is
  /// ill-formed.
  /// @tparam MsgId the type of the id parameter
  /// @tparam Params the types of the args parameters
  /// @param id an implementation-defined type identifying the message object
  /// @param args the arguments to add to the message
  template <typename MsgId, typename... Params>
  void Log(const MsgId& id, const Params&... args) noexcept {}

  template <typename... Attrs, typename MsgId, typename... Params>
  void LogWith(const std::tuple<Attrs...>& attrs, const MsgId& msg_id, const Params&... params) noexcept {}

  /// @brief Set log level threshold for this Logger instance.
  /// @param threshold the new threshold
  void SetThreshold(LogLevel threshold);

 private:
  Logger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel threshold);

  [[nodiscard]] const Key& GetKey() const;

  core::StringView CtxId() const;

  void Handle(std::shared_ptr<dlt::Message> message);

 private:
  friend class LoggerManager;
  friend class LogStream;
  struct Impl;
  std::shared_ptr<Impl> impl_;
};

/// @brief Creates a Logger object, holding the context which is registered in the Logging framework. If no model is
/// available the sink shall be the console per default.
/// @param ctx_id The context ID.
/// @param ctx_desc The description of the provided context ID.
/// @param ctx_def_log_level The default log level, set to Warning severity if not explicitly specified.
/// @return Reference to the internal managed instance of a Logger object. Ownership stays within the Logging framework.
Logger& CreateLogger(core::StringView ctx_id, core::StringView ctx_desc, LogLevel ctx_def_log_level = LogLevel::kWarn);

/// @brief Create a wrapper object for the given arguments.
/// Calling this function shall be ill-formed if any of these conditions are met:
/// - T is not an arithmetic type and not "bool" and not convertible to "ara::core::StringView" and not convertible to
/// "ara::core::Span<const ara::core::Byte>".
/// - T is convertible to "ara::core::StringView" or convertible to "ara::core::Span<const ara::core::Byte>" or "bool",
/// and "unit" is not "nullptr"
/// @param arg an argument payload object
/// @param name an optional "name" attribute for arg
/// @param unit an optional "unit" attribute for arg
/// @param format an optional formatting hint for integral or floating-point arguments; the default is to use the
/// implementation’s standard formatting
/// @return a wrapper object holding the supplied arguments
template <typename T>
Argument<T> Arg(T&& arg, const char* name = nullptr, const char* unit = nullptr, Format format = Dflt()) {
  return Argument<T>{};  // TODO
}
}  // namespace ara::log
#endif  // !VITO_AP_LOGGER_H_
