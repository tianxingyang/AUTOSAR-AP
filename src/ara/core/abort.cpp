#include "ara/core/abort.h"

#include <functional>
#include <iostream>
#include <mutex>

#include "ara/core/vector.h"

namespace ara::core {
class AbortMgr {
 public:
  static AbortMgr& Instance() {
    static AbortMgr singleton;
    return singleton;
  }

  AbortHandler SetAbortHandler(AbortHandler handler) noexcept {
    std::scoped_lock lock{mtx_};
    if (!handler) {
      handlers_.clear();
      handlers_.push_back(default_abort_handler_);
      return nullptr;
    }
    handlers_.push_back(handler);
    return handlers_.back();
  }

  bool AddAbortHandler(AbortHandler handler) noexcept {
    if (!handler) {
      return false;
    }

    std::scoped_lock lock{mtx_};
    if (handlers_.size() == kMaxAbortHandlerNum) {
      return false;
    }

    handlers_.push_back(handler);
    return true;
  }

  void Abort() { std::invoke(handlers_.back()); }

 private:
  AbortMgr() = default;

 private:
  std::mutex mtx_;
  Vector<AbortHandler> handlers_;
  static AbortHandler default_abort_handler_;
  static constexpr uint8_t kMaxAbortHandlerNum{8};
};

AbortHandler AbortMgr::default_abort_handler_{[]() noexcept { std::abort(); }};

namespace internal {
void Abort(StringView text) noexcept {
  std::cerr << text << '\n';
  AbortMgr::Instance().Abort();
}
}  // namespace internal
}  // namespace ara::core