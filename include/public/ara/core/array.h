#ifndef VITO_AP_ARRAY_H_
#define VITO_AP_ARRAY_H_

#include <array>
#include <cstdint>
#include <cstring>

namespace ara::core {

/// @brief Encapsulation of fixed size arrays.
/// @tparam T the type of element in the array
/// @tparam N the number of elements in the array
template <typename T, std::size_t N>
class Array final {
 public:
  /// @brief Alias type for a reference to an element.
  using reference = T&;
  /// @brief Alias type for a const_reference to an element.
  using const_reference = const T&;
  /// @brief The type of an iterator to elements.
  using iterator = T*;
  /// @brief The type of a const_iterator to elements.
  using const_iterator = const T*;
  /// @brief Alias for the type of parameters that indicate an index into the Array.
  using size_type = std::size_t;
  /// @brief Alias for the type of parameters that indicate a difference of indexes into the Array.
  using difference_type = std::ptrdiff_t;
  /// @brief Alias for the type of elements in this Array.
  using value_type = T;
  /// @brief Alias type for a pointer to an element.
  using pointer = T*;
  /// @brief Alias type for a pointer to a const element.
  using const_pointer = const T*;
  /// @brief The type of a reverse_iterator to elements.
  using reverse_iterator = std::reverse_iterator<iterator>;
  /// @brief The type of a const_reverse_iterator to elements.
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /// @brief Assign the given value to all elements of this Array.
  /// @param u the value;
  void fill(const T& u) { array_.fill(u); }

  /// @brief Exchange the contents of this Array with those of other.
  /// The noexcept specification shall make use of ADL for the swap() call.
  /// @param other the other Array
  void swap(Array& other) noexcept(noexcept(swap(std::declval<T&>(), std::declval<T&>()))) { array_.swap(other); }

  /// @brief Return an iterator pointing to the first element of this Array.
  /// @return the iterator
  iterator begin() noexcept { return array_.begin(); }

  /// @brief Return a const_iterator pointing to the first element of this Array.
  /// @return the const_iterator
  const_iterator begin() const noexcept { return array_.begin(); }

  /// @brief Return an iterator pointing past the last element of this Array.
  /// @return the iterator
  iterator end() noexcept { return array_.end(); }

  /// @brief Return a const_iterator pointing past the last element of this Array.
  /// @return the const_iterator
  const_iterator end() const noexcept { return array_.end(); }

  /// @brief Return a reverse_iterator pointing to the last element of this Array.
  /// @return the reverse_iterator
  reverse_iterator rbegin() noexcept { return array_.rbegin(); }

  /// @brief Return a const_reverse_iterator pointing to the last element of this Array.
  /// @return the const_reverse_iterator
  const_reverse_iterator rbegin() const noexcept { return array_.rbegin(); }

  /// @brief Return a reverse_iterator pointing past the first element of this Array.
  /// @return the reverse_iterator
  reverse_iterator rend() noexcept { return array_.rend(); }

  /// @brief Return a const_iterator pointing to the first element of this Array.
  /// @return the const_reverse_iterator
  const_reverse_iterator rend() const noexcept { return array_.rend(); }

  /// @brief Return a const_iterator pointing to the first element of this Array.
  /// @return the const_iterator
  const_iterator cbegin() const noexcept { return array_.cbegin(); }

  /// @brief Return a const_iterator pointing past the last element of this Array.
  /// @return the const_iterator
  const_iterator cend() const noexcept { return array_.cend(); }

  /// @brief Return a const_reverse_iterator pointing to the last element of this Array.
  /// @return the const_reverse_iterator
  const_reverse_iterator crbegin() const noexcept { return array_.crbegin(); }

  /// @brief Return a const_reverse_iterator pointing past the first element of this Array.
  /// @return the const_reverse_iterator
  const_reverse_iterator crend() const noexcept { return array_.crend(); }

  /// @brief Return the number of elements in this Array.
  /// @return N
  constexpr size_type size() const noexcept { return array_.size(); }

  /// @brief Return the maximum number of elements supported by this Array.
  /// @return N
  constexpr size_type max_size() const noexcept { return array_.max_size(); }

  /// @brief Return whether this Array is empty.
  /// @return true if this Array contains 0 elements, false otherwise
  constexpr bool empty() const noexcept { return array_.empty(); }

  /// @brief Return a reference to the n-th element of this Array.
  /// @param n the index into this Array
  /// @return the reference
  reference operator[](size_type n) { return array_[n]; }

  /// @brief Return a const_reference to the n-th element of this Array.
  /// @param n the index into this Array
  /// @return the const_reference
  constexpr const_reference operator[](size_type n) const { return array_[n]; }

  /// @brief Return a reference to the first element of this Array.
  /// The behavior of this function is undefined if the Array is empty.
  /// @return the reference
  reference front() { return array_.front(); }

  /// @brief Return a const_reference to the first element of this Array.
  /// The behavior of this function is undefined if the Array is empty.
  /// @return the reference
  constexpr const_reference front() const;

  /// @brief Return a reference to the last element of this Array.
  /// The behavior of this function is undefined if the Array is empty.
  /// @return the reference
  reference back() { return array_.back(); }

  /// @brief Return a const_reference to the last element of this Array.
  /// The behavior of this function is undefined if the Array is empty.
  /// @return the reference
  constexpr const_reference back() const { return array_.back(); }

  /// @brief Return a pointer to the first element of this Array.
  /// @return the pointer
  pointer data() noexcept { return array_.data(); }

  /// @brief Return a const_pointer to the first element of this Array.
  /// @return the const_pointer
  const_pointer data() const noexcept { return array_.data(); }

 private:
  std::array<T, N> array_;
};

/// @brief Return true if the two Arrays have equal content.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if the Arrays are equal, false otherwise
template <typename T, std::size_t N>
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return memcmp(lhs.data(), rhs.data(), N) == 0;
}

/// @brief Return true if the two Arrays have non-equal content.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if the Arrays are non-equal, false otherwise
template <typename T, std::size_t N>
bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return !(lhs == rhs);
}

/// @brief Return true if the contents of lhs are lexicographically less than the contents of rhs.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than rhs, false otherwise
template <typename T, std::size_t N>
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/// @brief Return true if the contents of rhs are lexicographically less than the contents of lhs.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if rhs is less than lhs, false otherwise
template <typename T, std::size_t N>
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return !(lhs < rhs || lhs == rhs);
}

/// @brief Return true if the contents of lhs are lexicographically less than or equal to the contents of rhs.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than or equal to rhs, false otherwise
template <typename T, std::size_t N>
bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return !(lhs > rhs);
}

/// @brief Return true if the contents of rhs are lexicographically less than or equal to the contents of lhs.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if rhs is less than or equal to lhs, false otherwise
template <typename T, std::size_t N>
bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
  return !(lhs < rhs);
}

/// @brief Overload of std::swap for ara::core::Array
/// @tparam T the type of element in the Arrays
/// @tparam N the number of elements in the Arrays
/// @param lhs the left-hand side of the call
/// @param rhs the right-hand side of the call
template <typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

template <std::size_t I, typename T, std::size_t N>
constexpr T& get(Array<T, N>& a) noexcept {
  return a[I];
}
}  // namespace ara::core

namespace std {
/// @brief Specialization of std::tuple_size for ara::core::Array.
/// This specialization shall meet the C++14 UnaryTypeTrait requirements with a Base Characteristic of
/// std::integral_constant<std::size_t, N>.
/// @tparam T the type of element in the Array
/// @tparam N the number of elements in the Array
template <typename T, size_t N>
struct tuple_size<ara::core::Array<T, N>> : public integral_constant<size_t, N> {};

/// @brief Specialization of std::tuple_element for ara::core::Array.
/// The implementation shall flag the condition I >= N as a compile error
/// @tparam T the type of element in the Array
/// @tparam I the index into the Array whose type is desired
/// @tparam N the number of elements in the Array
template <size_t I, typename T, size_t N>
struct tuple_element<I, ara::core::Array<T, N>> {
  static_assert(I < N, "index is out of bounds");
  using type = T;
};
}  // namespace std

#endif