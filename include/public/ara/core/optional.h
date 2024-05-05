#ifndef VITO_AP_OPTIONAL_H_
#define VITO_AP_OPTIONAL_H_

#include <optional>

namespace ara::core {
/// @brief A container with at most one element.
/// @tparam T the type of element in the container
template <typename T>
using Optional = std::optional<T>;

template <typename T>
inline void swap(Optional<T>& lhs, Optional<T>& rhs) {
  lhs.swap(rhs);
}
}  // namespace ara::core

#endif