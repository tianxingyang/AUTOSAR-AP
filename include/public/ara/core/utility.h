#ifndef VITO_AP_UTILITY_H_
#define VITO_AP_UTILITY_H_

#include <bits/range_access.h>

#include <cstddef>
#include <utility>

namespace ara::core {
/// @brief A non-integral binary type.
using Byte = std::byte;

/// @brief Denote an operation to be performed in-place.
/// An instance of this type can be passed to certain constructors of ara::core::Optional to denote the intention that
/// construction of the contained type shall be done in-place, i.e. without any copying taking place.
using in_place_t = std::in_place_t;

/// @brief The singleton instance of in_place_t.
inline constexpr in_place_t in_place{};

/// @brief Denote a type-distinguishing operation to be performed in-place.
/// An instance of this type can be passed to certain constructors of ara::core::Variant to denote the intention that
/// construction of the contained type shall be done in-place, i.e. without any copying taking place.
/// @tparam T -
template <typename T>
using in_place_type_t = std::in_place_type_t<T>;

/// @brief The singleton instances (one for each T) of in_place_type_t.
/// @tparam T the type to address
template <typename T>
inline constexpr in_place_type_t<T> in_place_type{};

/// @brief Denote an index-distinguishing operation to be performed in-place.
/// An instance of this type can be passed to certain constructors of ara::core::Variant to denote the intention that
/// construction of the contained type shall be done in-place, i.e. without any copying taking place.
/// @tparam I -
template <std::size_t I>
using in_place_index_t = std::in_place_index_t<I>;

/// @brief The singleton instances (one for each I) of in_place_index_t.
/// @tparam I the index to address
template <std::size_t I>
inline constexpr in_place_index_t<I> in_place_index{};

using std::data;
using std::empty;
using std::size;

}  // namespace ara::core

#endif