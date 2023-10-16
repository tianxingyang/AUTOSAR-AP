#ifndef VITO_AP_ERROR_DOMAIN_H_
#define VITO_AP_ERROR_DOMAIN_H_

#include <cstdint>

namespace ara::core {
class ErrorCode;

/// @brief Encapsulation of an error domain.
///
/// An error domain is the controlling entity for ErrorCode's error code values, and defines the mapping of such error
/// code values to textual representations. This class is a literal type, and subclasses are strongly advised to be
/// literal types as well.
class ErrorDomain {
 public:
  /// @brief Alias type for a unique ErrorDomain identifier type.
  using IdType = std::uint64_t;
  /// @brief Alias type for a domain-specific error code value.
  using CodeType = std::int32_t;
  /// @brief Alias type for vendor-specific supplementary data
  using SupportDataType = std::int32_t;

 public:
  /// @brief Copy construction shall be disabled.
  /// @param
  ErrorDomain(const ErrorDomain&) = delete;
  /// @brief Move construction shall be disabled.
  /// @param
  ErrorDomain(ErrorDomain&&) = delete;

  /// @brief Construct a new instance with the given identifier. Identifiers are expected to be system - wide unique
  /// @param id the unique identifier
  explicit constexpr ErrorDomain(IdType id) noexcept : id_{id} {}

  /// @brief Destructor.
  /// This dtor is non - virtual(and trivial) so that this class can be a
  /// literal type. While this class has virtual functions, no polymorphic
  /// destruction is needed.
  ~ErrorDomain() noexcept = default;  // NOLINT(clang-diagnostic-non-virtual-dtor)

  /// @brief Copy assignment shall be disabled.
  /// @param
  /// @return
  ErrorDomain& operator=(const ErrorDomain&) = delete;

  /// @brief Move assignment shall be disabled.
  /// @param
  /// @return
  ErrorDomain& operator=(ErrorDomain&&) = delete;

  /// @brief Compare for equality with another ErrorDomain instance. Two ErrorDomain instances compare equal when their
  /// identifiers(returned by Id()) are equal.
  /// @param other the other instance
  /// @return true if other is equal to *this, false otherwise
  constexpr bool operator==(const ErrorDomain& other) const noexcept { return Id() == other.Id(); }

  /// @brief Compare for non-equality with another ErrorDomain instance.
  /// @param other the other instance
  /// @return true if other is not equal to *this, false otherwise
  constexpr bool operator!=(const ErrorDomain& other) const noexcept { return !(*this == other); }

  /// @brief Return the unique domain identifier.
  /// @return the identifier
  constexpr IdType Id() const noexcept { return id_; }

  /// @brief Return the name of this error domain. The returned pointer remains owned by class ErrorDomain and shall not
  /// be freed by clients
  /// @return the name as a null-terminated string, never nullptr
  virtual const char* Name() const noexcept = 0;

  /// @brief Return a textual representation of the given error code. It is a Violation if the errorCode did not
  /// originate from this error domain, and thus be subject to SWS_CORE_00003.The returned pointer remains owned by the
  /// ErrorDomain subclass and shall not be freed by clients
  /// @param error_code the domain-specific error code
  /// @return the text as a null-terminated string, never nullptr
  virtual const char* Message(CodeType error_code) const noexcept = 0;

  /// @brief Throw the given error as exception.This function will determine the appropriate exception type for the
  /// given ErrorCode and throw it. The thrown exception will contain the given ErrorCode.
  /// @param error_code the ErrorCode
  virtual void ThrowAsException(const ErrorCode& error_code) const noexcept(false) = 0;

 private:
  IdType id_;
};
}  // namespace ara::core

#endif  // !VITO_AP_ERROR_DOMAIN_H_
