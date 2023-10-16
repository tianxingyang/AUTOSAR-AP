#ifndef VITO_AP_STRING_VIEW_H_
#define VITO_AP_STRING_VIEW_H_

#include <string_view>

namespace ara::core {
/// @brief A read-only view over a contiguous sequence of characters whose storage is owned by another object.
using StringView = std::string_view;
}  // namespace ara::core

#endif