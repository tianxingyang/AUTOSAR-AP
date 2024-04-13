#ifndef VITO_AP_STRING_H_
#define VITO_AP_STRING_H_

#include <string>

namespace ara::core {
/// @brief BasicString type.
/// @tparam Allocator the allocator to use for any memory allocations
template <typename Allocator = std::allocator<char>>
using BasicString = std::basic_string<char, std::char_traits<char>, Allocator>;

using String = BasicString<>;
}  // namespace ara::core

#endif