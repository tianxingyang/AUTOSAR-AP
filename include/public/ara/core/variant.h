#ifndef VITO_AP_VARIANT_H_
#define VITO_AP_VARIANT_H_

#include <variant>

namespace ara::core {
/// @brief A type-safe union.
/// @tparam ...Types the types that the Variant is able to hold
template <typename... Types>
using Variant = std::variant<Types...>;

/// @brief Exchange the state of lhs with that of rhs.
/// @tparam ...Types the types that the Variant is able to hold
/// @param lhs the first Variant
/// @param rhs the second Variant
template <typename... Types>
inline void swap(Variant<Types...>& lhs, Variant<Types...> rhs) {
  lhs.swap(rhs);
}
}  // namespace ara::core

#endif