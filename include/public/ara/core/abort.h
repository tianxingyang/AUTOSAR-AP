#ifndef VITO_AP_ABORT_H_
#define VITO_AP_ABORT_H_

#include "fmt/format.h"

namespace ara::core {
/// @brief A function declaration with the correct prototype for SetAbortHandler().
/// This declaration exists only for providing a function type that includes "noexcept" and that acts as base type for a
/// type alias, which is defined in SWS_CORE_00050.
/// This compensates for the fact that the C++ standard (up to and including C++14) prohibits that "noexcept" appears in
/// an alias-declaration.
/// There is no implementation of this function.
void AbortHandlerPrototype() noexcept;

/// @brief The type of a handler for SetAbortHandler().
using AbortHandler = decltype(&AbortHandlerPrototype);

/// @brief Add a custom Abort handler function and return the most recently added one.
/// By setting nullptr, the implementation may restore the default handler instead; this will remove all previously
/// installed handlers. This function can be called from multiple threads simultaneously; these calls are performed in
/// an implementation-defined sequence.
/// @param handler a custom Abort handler (or nullptr)
/// @return the most recently installed Abort handler (or nullptr if none was installed)
AbortHandler SetAbortHandler(AbortHandler handler) noexcept;

/// @brief Add a custom Abort handler function.
/// false is returned when either the implementation-defined limit for number of abort handlers would be exceeded, or if
/// nullptr is passed to this function
/// Implementations support at least 8 AbortHandlers.
/// @param handler a custom Abort handler
/// @return true if the given handler was successfully installed; false otherwise
bool AddAbortHandler(AbortHandler handler) noexcept;

/// @brief Abort the current operation.
/// This function will never return to its caller. The stack is not unwound: destructors of variables with automatic
/// storage duration are not called.
/// Calling this function is ill-formed if any of the arguments is not convertible to ara::core::StringView.
/// @tparam ...Args the types of arguments given to this function
/// @param ...args custom texts to be added in the log message being output with automatic storage duration are not
/// called.
template <typename... Args>
void Abort(const Args&... args) noexcept {}

namespace internal {
/// @brief abort process
/// @param text abort message
void Abort(const char* text) noexcept;
}  // namespace internal

}  // namespace ara::core

#endif