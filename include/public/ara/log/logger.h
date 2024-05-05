#ifndef VITO_AP_LOGGER_H_
#define VITO_AP_LOGGER_H_

#include <cstdint>

#include "ara/log/log_stream.h"

namespace ara::log {
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

}  // namespace ara::log
#endif  // !VITO_AP_LOGGER_H_
