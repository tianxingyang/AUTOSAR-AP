#ifndef VITO_AP_SKELETON_BASE_H_
#define VITO_AP_SKELETON_BASE_H_

#include "ara/core/result.h"

namespace ara::com {
class SkeletonBase {
 public:
  virtual core::Result<void> OfferService() = 0;
};
}  // namespace ara::com

#endif