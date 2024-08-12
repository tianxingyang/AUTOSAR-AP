#include "ara/com/skeleton_base.h"

namespace ara::com {
core::Result<void> SkeletonBase::OfferService() { return core::Result<void>::Ok(); }
}  // namespace ara::com