#include "ara/com/skeleton_base.h"

#include "ara/com/com_error_domain.h"

namespace ara::com {
core::Result<void> SkeletonBase::OfferService() { return {}; }
}  // namespace ara::com