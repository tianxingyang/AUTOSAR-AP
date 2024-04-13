#ifndef VITO_AP_MAP_H_
#define VITO_AP_MAP_H_

#include <map>

namespace ara::core {
/// @brief An ordered associative array.
/// @tparam K the type of keys in the map
/// @tparam V the type of values in the map
/// @tparam C the comparator for key equality tests
/// @tparam Allocator the allocator to use for any memory allocations
template <typename K, typename V, typename C = std::less(K), typename Allocator = std::allocator<std::pair<const K, V>>>
using Map = std::map<K, V, C, Allocator>;

/// @brief Exchange the state of lhs with that of rhs.
/// @tparam K the type of keys in the map
/// @tparam V the type of values in the map
/// @tparam C the comparator for key equality tests
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the first Map
/// @param rhs the second Map
template <typename K, typename V, typename C = std::less(K), typename Allocator = std::allocator<std::pair<const K, V>>>
void swap(Map<K, V, C, Allocator>& lhs, Map<K, V, C, Allocator>& rhs) {
  lhs.swap(rhs);
}
}  // namespace ara::core

#endif
