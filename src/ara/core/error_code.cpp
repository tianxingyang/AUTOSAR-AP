#include "ara/core/error_code.h"

#include "ara/core/core_error_domain.h"
#include "ara/core/exception.h"
#include "ara/core/result.h"

namespace ara::core {
StringView ErrorCode::Message() const noexcept { return domain_->Message(value_); }

void ErrorCode::ThrowAsException() const {}
}  // namespace ara::core