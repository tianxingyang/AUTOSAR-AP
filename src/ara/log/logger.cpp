#include "ara/log/logger.h"

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
}  // namespace ara::log