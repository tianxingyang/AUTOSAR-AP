#ifndef VITO_AP_INSTANCE_IDENTIFIER_H_
#define VITO_AP_INSTANCE_IDENTIFIER_H_

#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/

namespace ara::com {
class InstanceIdentifier {
 public:
  static core::Result<InstanceIdentifier> Create(core::StringView serialized_format) noexcept;

  explicit InstanceIdentifier(core::StringView serialized_format) noexcept;

  core::StringView ToString() const;

  bool operator==(const InstanceIdentifier& other) const;

  bool operator<(const InstanceIdentifier& other) const;
  
  InstanceIdentifier& operator=(const InstanceIdentifier& other);
};
}  // namespace ara::com

#endif