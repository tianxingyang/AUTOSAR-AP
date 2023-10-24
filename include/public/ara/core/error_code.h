#ifndef VITO_AP_ERROR_CODE_H_
#define VITO_AP_ERROR_CODE_H_

#include "ara/core/error_domain.h"
#include "ara/core/string_view.h"

namespace ara::core {
/// @brief Encapsulation of an error code. An ErrorCode contains a raw error code value and an error domain.
/// The raw error code value is specific to this error domain.
class ErrorCode final {
 public:
  /// @brief Construct a new ErrorCode instance with parameters.
  /// This constructor does not participate in overload resolution unless EnumT is an enum type.
  /// @tparam EnumT an enum type that contains error code values
  /// @param e a domain-specific error code value
  /// @param data optional vendor-specific supplementary error context data
  // template <typename EnumT, std::enable_if_t<std::is_enum_v<EnumT>> = true>
  // constexpr ErrorCode(EnumT e, ErrorDomain::SupportDataType data = ErrorDomain::SupportDataType{}) noexcept
  //     : value_{static_cast<ErrorDomain::CodeType>(e)}, data_{data} {}

  /// @brief Construct a new ErrorCode instance with parameters.
  /// @param value a domain-specific error code value
  /// @param domain the ErrorDomain associated with value
  /// @param data optional vendor-specific supplementary error context data
  constexpr ErrorCode(ErrorDomain::CodeType value, const ErrorDomain& domain,
                      ErrorDomain::SupportDataType data = ErrorDomain::SupportDataType{}) noexcept
      : value_{value}, data_{data}, domain_{&domain} {}

  /// @brief Return the raw error code value.
  /// @return the raw error code value
  constexpr ErrorDomain::CodeType Value() const noexcept { return value_; }

  /// @brief Return the domain with which this ErrorCode is associated.
  /// @return the ErrorDomain
  constexpr const ErrorDomain& Domain() const noexcept { return *domain_; }

  /// @brief Return the supplementary error context data.
  /// The underlying type and the meaning of the returned value are implementation - defined.
  /// @return the supplementary error context data
  constexpr ErrorDomain::SupportDataType SupportData() const noexcept { return data_; }

  /// @brief Return a textual representation of this ErrorCode.
  /// @return the error message text
  StringView Message() const noexcept;

  /// @brief Throw this error as exception.
  /// This function will determine the appropriate exception type for this ErrorCode and throw it.
  /// The thrown exception will contain this ErrorCode.
  void ThrowAsException() const;

 private:
  ErrorDomain::CodeType value_;
  ErrorDomain::SupportDataType data_;
  const ErrorDomain* domain_;
};

/// @brief Non-member operator== for ErrorCode.
/// Two ErrorCode instances compare equal if the results of their Value() and Domain() functions
/// are equal. The result of SupportData() is not considered for equality.
/// @param lhs the left hand side of the comparison
/// @param rhs the right hand side of the comparison
/// @return true if the two instances compare equal, false otherwise
constexpr bool operator==(const ErrorCode& lhs, const ErrorCode& rhs) noexcept {
  return lhs.Domain() == rhs.Domain() && lhs.Value() == rhs.Value();
}

/// @brief Non-member operator!= for ErrorCode.
/// Two ErrorCode instances compare equal if the results of their Value() and Domain() functions
/// are equal. The result of SupportData() is not considered for equality.
/// @param lhs the left hand side of the comparison
/// @param rhs the right hand side of the comparison
/// @return true if the two instances compare not equal, false otherwise
constexpr bool operator!=(const ErrorCode& lhs, const ErrorCode& rhs) noexcept { return !(lhs == rhs); }

}  // namespace ara::core

#endif