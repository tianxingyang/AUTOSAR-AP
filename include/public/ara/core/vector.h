#ifndef VITO_AP_VECTOR_H_
#define VITO_AP_VECTOR_H_

#include <vector>

namespace ara::core {
/// @brief A growable container for contiguous elements.
/// @tparam T the type of element in the vector
/// @tparam Allocator the allocator to use for any memory allocations
template <typename T, typename Allocator = std::allocator<T>>
using Vector = std::vector<T, Allocator>;

/// @brief Return true if the two Vectors have equal content.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if the Vectors are equal, false otherwise
template <typename T, typename Allocator = std::allocator<T>>
bool operator==(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return lhs == rhs;
}

/// @brief Return true if the two Vectors have non-equal content.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if the Vectors are non-equal, false otherwise
template <typename T, typename Allocator = std::allocator<T>>
bool operator!=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return !(lhs == rhs);
}

/// @brief Return true if the contents of lhs are lexicographically less than the contents of rhs.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than rhs, false otherwise
template <typename T, typename Allocator = std::allocator<T>>
bool operator<(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return lhs < rhs;
}

/// @brief Return true if the contents of lhs are lexicographically less than or equal to the contents of rhs.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than or equal to rhs, false otherwise
template <typename T, typename Allocator = std::allocator<T>>
bool operator<=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return lhs < rhs || lhs == rhs;
}

/// @brief Return true if the contents of lhs are lexicographically greater than the contents of rhs.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than rhs, false otherwise
template <typename T, typename Allocator = std::allocator<T>>
bool operator>(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return !(lhs <= rhs);
}

/// @brief Return true if the contents of lhs are lexicographically greater than or equal to the contents of rhs.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than or equal to rhs, false otherwise
template <typename T, typename Allocator = std::allocator<T>>
bool operator>=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return !(lhs < rhs);
}

/// @brief Exchange the state of lhs with that of rhs.
/// @tparam T the type of element in the Vector
/// @tparam Allocator the allocator to use for any memory allocations
/// @param lhs the first Vector
/// @param rhs the second Vector
template <typename T, typename Allocator = std::allocator<T>>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) {
  lhs.swap(rhs);
}
}  // namespace ara::core

#endif