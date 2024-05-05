#ifndef VITO_AP_LOG_STREAM_BUFFER_H_
#define VITO_AP_LOG_STREAM_BUFFER_H_

#include <cstdint>

#include "ara/core/array.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/log/log_error_domain.h"

namespace ara::log {
class Buffer {
 public:
  template <typename T>
  core::Result<void> Append(T value) {
    using R = core::Result<void>;
    if (IsOverflow(value)) {
      return R::FromError(LogErrc::kBufferOverflow);
    }
    DoAppend(value);
    ExpandSize(value);
    return {};
  }

 private:
  template <typename T>
  bool IsOverflow(T value) {
    return length_ + sizeof value > kMaxBufferSize;
  }

  template <>
  bool IsOverflow(core::StringView value) {
    return length_ + value.size() > kMaxBufferSize;
  }

  template <>
  bool IsOverflow(core::Span<const core::Byte> value) {
    return length_ + value.size() > kMaxBufferSize;
  }

  template <typename T>
  void ExpandSize(T) {
    length_ += sizeof(T);
  }

  template <>
  void ExpandSize(core::StringView value) {
    length_ += value.size();
  }

  template <>
  void ExpandSize(core::Span<const core::Byte> value) {
    length_ += value.size();
  }

  template <typename T>
  void DoAppend(T value) {
    std::memcpy(buffer_.data() + length_, &value, sizeof value);
  }

  template <>
  void DoAppend(core::StringView value) {
    std::memcpy(buffer_.data() + length_, value.data(), value.size());
  }

  template <>
  void DoAppend(core::Span<const core::Byte> value) {
    std::memcpy(buffer_.data() + length_, value.data(), value.size());
  }

 private:
  static constexpr std::uint16_t kMaxBufferSize{1 << 11};
  core::Array<core::Byte, kMaxBufferSize> buffer_{};
  std::uint16_t length_{0};
};
}  // namespace ara::log

#endif  // !VITO_AP_LOG_STREAM_BUFFER_H_
