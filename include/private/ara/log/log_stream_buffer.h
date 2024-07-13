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

  template <typename T>
  void ExpandSize(T) {
    length_ += sizeof(T);
  }

  template <typename T>
  void DoAppend(T value) {
    std::memcpy(buffer_.data() + length_, &value, sizeof value);
  }

 private:
  static constexpr std::uint16_t kMaxBufferSize{1 << 11};
  core::Array<core::Byte, kMaxBufferSize> buffer_{};
  std::uint16_t length_{0};
};

template <>
inline bool Buffer::IsOverflow(core::StringView value) {
  return length_ + value.size() > kMaxBufferSize;
}

template <>
inline bool Buffer::IsOverflow(core::Span<const core::Byte> value) {
  return length_ + value.size() > kMaxBufferSize;
}

template <>
inline void Buffer::ExpandSize(core::StringView value) {
  length_ += value.size();
}

template <>
inline void Buffer::ExpandSize(core::Span<const core::Byte> value) {
  length_ += value.size();
}

template <>
inline void Buffer::DoAppend(core::StringView value) {
  std::memcpy(buffer_.data() + length_, value.data(), value.size());
}

template <>
inline void Buffer::DoAppend(core::Span<const core::Byte> value) {
  std::memcpy(buffer_.data() + length_, value.data(), value.size());
}
}  // namespace ara::log

#endif  // !VITO_AP_LOG_STREAM_BUFFER_H_
