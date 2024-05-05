#ifndef VITO_AP_SCALE_LINEAR_AND_TEXTTABLE_H_
#define VITO_AP_SCALE_LINEAR_AND_TEXTTABLE_H_

#include <type_traits>

namespace ara::core {
/// @brief A wrapper type extending the capabilities of an enum.
/// The definitions of this class have been carefully set up so that the behavior of this class is the same as that of a
/// regular enum type in C++17.
/// The type T is required to be an enum type.
/// @tparam T the type of the enum
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
class ScaleLinearAndTexttable final {
 public:
  /// @brief The enum's underlying type.
  using UnderlyingType = std::underlying_type_t<T>;

  /// @brief Default constructor.
  /// As with built-in enum types, this constructor leaves the value in an indeterminate state.
  constexpr ScaleLinearAndTexttable() noexcept = default;

  /// @brief Copy constructor.
  /// @param other the other instance
  constexpr ScaleLinearAndTexttable(const ScaleLinearAndTexttable& other) noexcept = default;

  /// @brief Move constructor.
  /// @param other the other instance
  constexpr ScaleLinearAndTexttable(ScaleLinearAndTexttable&& other) noexcept = default;

  /// @brief Create an instance from a value of the enum.
  /// @param v a value from the enum
  constexpr ScaleLinearAndTexttable(const T& v) noexcept : v_{static_cast<UnderlyingType>(v)} {}

  /// @brief Create an instance from a value of the enum's underlying type.
  /// @param v a value from the enum's underlying type
  explicit constexpr ScaleLinearAndTexttable(const UnderlyingType& v) noexcept : v_{v} {}

  /// @brief Copy assignment operator.
  /// @param other the other instance
  /// @return *this
  constexpr ScaleLinearAndTexttable& operator=(const ScaleLinearAndTexttable& other) noexcept = default;

  /// @brief Move assignment operator.
  /// @param other the other instance
  /// @return *this
  constexpr ScaleLinearAndTexttable& operator=(ScaleLinearAndTexttable&& other) noexcept = default;

  /// @brief Assign the given enum value to this instance.
  /// @param v the enum value
  /// @return *this
  constexpr ScaleLinearAndTexttable& operator=(const T& v) noexcept {
    v_ = static_cast<UnderlyingType>(v);
    return *this;
  }

  /// @brief Destructor.
  ~ScaleLinearAndTexttable() noexcept = default;

  /// @brief Conversion operator to the enum's underlying type.
  /// @return the value
  explicit constexpr operator UnderlyingType() const noexcept { return v_; }

 private:
  UnderlyingType v_;
};

/// @brief Return true if the numerical value of lhs is equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator==(const ScaleLinearAndTexttable<T>& lhs,
                                 const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs == rhs;
}

/// @brief Return true if the numerical value of lhs is equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator==(const ScaleLinearAndTexttable<T>& lhs, const T& rhs) noexcept {
  return lhs == rhs;
}

/// @brief Return true if the numerical value of lhs is equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator==(const T& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs == rhs;
}

/// @brief Return true if the numerical value of lhs is not equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is not equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
constexpr bool operator!=(const ScaleLinearAndTexttable<T>& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs != rhs;
}

/// @brief Return true if the numerical value of lhs is not equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is not equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator!=(const ScaleLinearAndTexttable<T>& lhs, const T& rhs) noexcept {
  return lhs != rhs;
}

/// @brief Return true if the numerical value of lhs is not equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is not equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator!=(const T& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs != rhs;
}

/// @brief Return true if the numerical value of lhs is less than the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator<(const ScaleLinearAndTexttable<T>& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs < rhs;
}

/// @brief Return true if the numerical value of lhs is less than the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator<(const ScaleLinearAndTexttable<T>& lhs, const T& rhs) noexcept {
  return lhs < rhs;
}

/// @brief Return true if the numerical value of lhs is less than the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator<(const T& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs < rhs;
}

/// @brief Return true if the numerical value of lhs is less than or equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than or equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator<=(const ScaleLinearAndTexttable<T>& lhs,
                                 const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs <= rhs;
}

/// @brief Return true if the numerical value of lhs is less than or equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than or equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator<=(const ScaleLinearAndTexttable<T>& lhs, const T& rhs) noexcept {
  return lhs <= rhs;
}

/// @brief Return true if the numerical value of lhs is less than or equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is less than or equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator<=(const T& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs <= rhs;
}

/// @brief Return true if the numerical value of lhs is greater than the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator>(const ScaleLinearAndTexttable<T>& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs > rhs;
}

/// @brief Return true if the numerical value of lhs is greater than the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator>(const ScaleLinearAndTexttable<T>& lhs, const T& rhs) noexcept {
  return lhs > rhs;
}

/// @brief Return true if the numerical value of lhs is greater than the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator>(const T& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs > rhs;
}

/// @brief Return true if the numerical value of lhs is greater than or equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than or equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator>=(const ScaleLinearAndTexttable<T>& lhs,
                                 const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs >= rhs;
}

/// @brief Return true if the numerical value of lhs is greater than or equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than or equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator>=(const ScaleLinearAndTexttable<T>& lhs, const T& rhs) noexcept {
  return lhs >= rhs;
}

/// @brief Return true if the numerical value of lhs is greater than or equal to the numerical value of rhs.
/// @tparam T the type of the enum value
/// @param lhs the left-hand side of the comparison
/// @param rhs the right-hand side of the comparison
/// @return true if lhs is greater than or equal to rhs, false otherwise
template <typename T, std::enable_if_t<std::is_enum_v<T>, bool> = true>
inline constexpr bool operator>=(const T& lhs, const ScaleLinearAndTexttable<T>& rhs) noexcept {
  return lhs >= rhs;
}
}  // namespace ara::core

namespace std {
/// @brief Specialization of std::hash for ara::core::ScaleLinearAndTexttable.
/// @tparam T tye type of the enum
template <typename T>
struct hash<ara::core::ScaleLinearAndTexttable<T>> {
  /// @brief Calculate a hash value for the given ScaleLinearAndTexttable.
  /// @param v the ScaleLinearAndTexttable
  /// @return the hash value
  size_t operator()(ara::core::ScaleLinearAndTexttable<T> const& v) const noexcept {
    return std::hash<typename ara::core::ScaleLinearAndTexttable<T>::UnderlyingType>()(v());
  }
};
}  // namespace std

#endif