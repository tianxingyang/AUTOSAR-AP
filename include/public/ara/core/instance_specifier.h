#ifndef VITO_AP_INSTANCE_SPECIFIER_H_
#define VITO_AP_INSTANCE_SPECIFIER_H_

#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"

namespace ara::core {
/// @brief class representing an AUTOSAR Instance Specifier, which is basically an AUTOSAR shortname - path wrapper.
class InstanceSpecifier final {
 public:
  /// @brief throwing ctor from meta-model string
  /// @param meta_model_identifier string representation of a valid InstanceSpecifier, according to the syntax rules
  /// given by SWS_CORE_10200 and SWS_CORE_10203.
  /// @exception CoreException in case the given metaModelIdentifier is not a valid meta-model identifier/short name
  /// path
  explicit InstanceSpecifier(StringView meta_model_identifier) : meta_model_identifier_{meta_model_identifier} {}

  /// @brief Copy constructor
  /// @param other the other instance
  InstanceSpecifier(const InstanceSpecifier& other) = default;

  /// @brief Move constructor
  /// @param other the other instance
  InstanceSpecifier(InstanceSpecifier&& other) noexcept = default;

  /// @brief Copy assignment operator
  /// @param other the other instance
  /// @return *this
  InstanceSpecifier& operator=(const InstanceSpecifier& other) = default;

  /// @brief Move assignment operator
  /// @param other the other instance
  /// @return *this
  InstanceSpecifier& operator=(InstanceSpecifier&& other) = default;

  /// @brief Destructor
  ~InstanceSpecifier() noexcept = default;

  /// @brief Create a new instance of this class.
  /// @param meta_model_identifier string representation of a valid InstanceSpecifier, according to the syntax rules
  /// given by SWS_CORE_10200 and SWS_CORE_10203.
  /// @errors CoreErrc::kInvalidMetaModelShortname if any of the path elements of metaModelIdentifier is missing or
  /// contains invalid characters
  /// @errors CoreErrc::kInvalidMetaModelPath if the metaModelIdentifier is not a valid path to a model element
  /// @return a Result, containing either a syntactically valid InstanceSpecifier, or an ErrorCode
  static Result<InstanceSpecifier> Create(StringView meta_model_identifier) noexcept {
    using R = Result<InstanceSpecifier>;
    return R::FromValue(InstanceSpecifier{meta_model_identifier});
  }

  /// @brief eq operator to compare with other InstanceSpecifier instance.
  /// @param other InstanceSpecifier instance to compare this one with.
  /// @return true in case both InstanceSpecifiers are denoting exactly the same model element, false otherwise.
  bool operator==(const InstanceSpecifier& other) const noexcept {
    return meta_model_identifier_ == other.meta_model_identifier_;
  }

  /// @brief eq operator to compare with other InstanceSpecifier instance.
  /// @param other string representation to compare this one with.
  /// @return true in case this InstanceSpecifier is denoting exactly the same model element as other, false otherwise.
  bool operator==(StringView other) const noexcept { return meta_model_identifier_ == other; }

  /// @brief uneq operator to compare with other InstanceSpecifier instance.
  /// @param other InstanceSpecifier instance to compare this one with.
  /// @return false in case both InstanceSpecifiers are denoting exactly the same model element, true otherwise.
  bool operator!=(const InstanceSpecifier& other) const noexcept { return !(*this == other); }

  /// @brief uneq operator to compare with other InstanceSpecifier string representation.
  /// @param other string representation to compare this one with.
  /// @return false in case this InstanceSpecifier is denoting exactly the same model element as other, true otherwise.
  bool operator!=(StringView other) const noexcept { return !(*this == other); }

  /// @brief lower than operator to compare with other InstanceSpecifier for ordering purposes (f.i. when collecting
  /// identifiers in maps).
  /// @param other InstanceSpecifier instance to compare this one with.
  /// @return true in case this InstanceSpecifier is lexically lower than other, false otherwise.
  bool operator<(const InstanceSpecifier& other) const noexcept {
    return meta_model_identifier_ < other.meta_model_identifier_;
  }

  /// @brief method to return the stringified form of InstanceSpecifier
  /// @return stringified form of InstanceSpecifier. Lifetime of the underlying string is only guaranteed for the
  /// lifetime of the underlying string of the StringView passed to the constructor.
  StringView ToString() const noexcept { return meta_model_identifier_; }

 private:
  String meta_model_identifier_;
};

/// @brief Non-member function operator== to allow StringView on lhs.
/// @param lhs stringified form of a InstanceSpecifier
/// @param rhs an InstanceSpecifier
/// @return true in case rhs string representation equals lhs
inline bool operator==(StringView lhs, const InstanceSpecifier& rhs) noexcept { return rhs == lhs; }

/// @brief Non-member function operator!= to allow StringView on lhs.
/// @param lhs stringified form of a InstanceSpecifier
/// @param rhs an InstanceSpecifier
/// @return true in case rhs string representation not equals lhs
inline bool operator!=(StringView lhs, const InstanceSpecifier& rhs) noexcept { return rhs != lhs; }
}  // namespace ara::core

#endif