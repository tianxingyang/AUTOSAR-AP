#include "ara/core/abort.h"

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
      return nullptr;
    }
    handlers_.push_back(handler);
    return handlers_.back();
  }

  bool AddAbortHandler(AbortHandler handler) noexcept {
    std::scoped_lock lock{mtx_};
    if (handlers_.size() == kMaxAbortHandlerNum) {
      return false;
    }

    handlers_.push_back(handler);
    return true;
  }

  template <typename... Args>
  void Abort(const Args&... args) noexcept {
    if (handlers_.empty()) {
      
    }
  }

 private:
  AbortMgr() = default;

 private:
  std::mutex mtx_;
  Vector<AbortHandler> handlers_;
  static AbortHandler default_abort_handler_;
  static constexpr uint8_t kMaxAbortHandlerNum{8};
};

AbortHandler AbortMgr::default_abort_handler_{[]() noexcept { abort(); }};
}  // namespace ara::core