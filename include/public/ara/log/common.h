#ifndef VITO_AP_COMMON_H_
#define VITO_AP_COMMON_H_

#include <cstdint>

namespace ara::log {
/// @brief List of possible severity levels.
enum class LogLevel : std::uint8_t {
  /// @brief No logging.
  kOff = 0x00,
  /// @brief Fatal error, not recoverable.
  kFatal = 0x01,
  /// @brief Error with impact to correct functionality.
  kError = 0x02,
  /// @brief Warning if correct behavior cannot be ensured.
  kWarn = 0x03,
  /// @brief Informational, providing high level understanding.
  kInfo = 0x04,
  /// @brief Detailed information for programmers.
  kDebug = 0x05,
  /// @brief Extra-verbose debug messages(highest grade of information)
  kVerbose = 0x06,
};

/// @brief Client state representing the connection state of an external client.
enum class ClientState : std::int8_t {
  /// @brief DLT back-end not up and running yet, state cannot be determined.
  kUnknown = -1,
  /// @brief No remote client detected.
  kNotConnected,
  /// @brief Remote client is connected.
  kConnected,
};

class LoggingHandler {
 public:
  virtual ~LoggingHandler() = default;
  virtual void Emit() = 0;
};
}  // namespace ara::log

#endif  // !VITO_AP_COMMON_H_
