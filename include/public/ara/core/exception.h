#ifndef VITO_AP_EXCEPTION_H_
#define VITO_AP_EXCEPTION_H_

#include <exception>

#include "ara/core/error_code.h"

namespace ara::core {
/// @brief Base type for all AUTOSAR exception types.
class Exception : public std::exception {
 public:
  /// @brief Construct a new Exception object with a specific ErrorCode.
  /// @param err the ErrorCode
  explicit Exception(ErrorCode err) noexcept : ec_{err} {}

  /// @brief Move constructor from another instance.
  /// @param other the other instance
  Exception(Exception&& other) = default;

  /// @brief Move assignment operator from another instance.
  /// @param other the other instance
  /// @return -
  Exception& operator=(Exception&& other) & = default;

  /// @brief Destructs the Exception object.
  ~Exception() override = default;

  /// @brief Return the explanatory string.
  /// This function overrides the virtual function std::exception::what. All guarantees about the
  /// lifetime of the returned pointer that are given for std::exception::what are preserved.
  /// @return a null-terminated string
  const char* what() const noexcept override { return Error().Message().data(); }

  /// @brief Return the embedded ErrorCode that was given to the constructor.
  /// @return reference to the embedded ErrorCode
  const ErrorCode& Error() const noexcept { return ec_; }

 protected:
  /// @brief Copy assignment operator from another instance.
  /// This function is "protected" in order to prevent some opportunities for accidental slicing.
  /// @param other the other instance
  /// @return *this
  Exception& operator=(const Exception& other) = default;

  /// @brief Copy constructor from another instance.
  /// This function is "protected" in order to prevent some opportunities for accidental slicing.
  /// @param other the other instance
  Exception(const Exception& other) = default;

 private:
  ErrorCode ec_;
};
}  // namespace ara::core
#endif