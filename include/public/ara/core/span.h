#ifndef VITO_AP_SPAN_H_
#define VITO_AP_SPAN_H_

#include <limits>
#include <span>

namespace ara::core {

constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

template <typename T, std::size_t Extent = dynamic_extent>
using Span = std::span<T, Extent>;
}  // namespace ara::core

#endif