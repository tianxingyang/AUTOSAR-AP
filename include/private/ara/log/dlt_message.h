#ifndef VITO_AP_DLT_MESSAGE_H_
#define VITO_AP_DLT_MESSAGE_H_

#include <bitset>
#include <cstring>
#include <memory>
#include <thread>
#include <type_traits>
#include <variant>

#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/log/common.h"

namespace ara::log::dlt {
constexpr std::uint8_t kHtyp2Len{32};

class HeaderType {
 public:
  static HeaderType VerboseMode();

  enum class Cnti : std::uint8_t {
    kVerboseModeDataMessage = 0x0,
    kNonVerboseModeDataMessage = 0x1,
    kControlMessage = 0x2,
    kReserved = 0x3,
  };

  void SetContentInfo(Cnti cnti);

  Cnti GetContentInfo() const;

  void SetWithEcuId(bool with);

  bool GetWithEcuId() const;

  void SetWithAppAndCtxId(bool with);

  bool GetWithAppAndCtxId() const;

  void SetWithSessionId(bool with);

  bool GetWithSessionId() const;

  void SetWithSourceFileNameAndLine(bool with);

  bool GetWithSourceFileNameAndLine() const;

  void SetWithTags(bool with);

  bool GetWithTags() const;

  void SetWithPrivacyLevel(bool with);

  bool GetWithPrivacyLevel() const;

  void SetWithSegmentation(bool with);

  bool GetWithSegmentation() const;

 private:
  HeaderType();

 private:
  std::bitset<kHtyp2Len> value_;
};

constexpr std::uint8_t kMsinLen{8};
class MessageInfo {
  enum class MessageType : std::uint8_t {
    kLog = 0x0,
    kTrace = 0x1,
    kNetwork = 0x2,
    kControl = 0x3,
  };

  enum class LogMessageInfo : std::uint8_t {
    kFatal = 0x1,
    kError = 0x2,
    kWarn = 0x3,
    kInfo = 0x4,
    kDebug = 0x5,
    kVerbose = 0x6,
  };

  enum class TraceMessageInfo : std::uint8_t {
    kVariable = 0x1,
    kFunctionIn = 0x2,
    kFunctionOut = 0x3,
    kState = 0x4,
    kVfb = 0x5,
  };

  enum class NetworkMessageInfo : std::uint8_t {
    kIpc = 0x1,
    kCan = 0x2,
    kFlexray = 0x3,
    kMost = 0x4,
    kEthernet = 0x5,
    kSomeIp = 0x6,
  };

  enum class ControlMessageInfo : std::uint8_t {
    kRequest = 0x1,
    kResponse = 0x2,
  };

 public:
  static MessageInfo LogMessage(LogLevel log_level);

  static MessageInfo TraceMessage(TraceMessageInfo trace_info);

  static MessageInfo NetworkMessage(NetworkMessageInfo network_info);

  LogLevel GetLogLevel() const;

 private:
  MessageInfo(MessageType message_type, std::uint8_t message_type_info);

 private:
  std::bitset<kMsinLen> value_;
};

class Timestamp {
 public:
  Timestamp();

  const core::String ToString() const;

 private:
  std::uint32_t nanoseconds_;
  std::uint64_t seconds_;
};

class BaseHeader {
 public:
  static BaseHeader VerboseModeLogBaseHeader(HeaderType&& header_type, LogLevel log_level);

  LogLevel GetLogLevel() const;

  core::String GetTimeStr() const;

 private:
  explicit BaseHeader(HeaderType&& header_type);

 private:
  HeaderType header_type_;
  static std::atomic_uint8_t message_counter_;
  std::uint8_t this_counter_{message_counter_++};
  [[maybe_unused]] std::uint16_t message_length_{0};
  core::Optional<MessageInfo> message_info_;
  core::Optional<std::uint8_t> number_of_arguments_;
  core::Optional<Timestamp> timestamp_;
  core::Optional<std::uint32_t> msid_;
};

class ExtensionHeader {
 public:
  ExtensionHeader() = default;

  void SetEcuId(core::StringView ecu_id);

  core::StringView EcuId() const;

  void SetAppId(core::StringView app_id);

  core::StringView AppId() const;

  void SetCtxId(core::StringView ctx_id);

  core::StringView CtxId() const;

 private:
  template <typename DT, typename VT>
  struct Field {
    core::Optional<DT> desc;
    core::Optional<VT> value;
  };

  Field<uint8_t, core::String> ecu_id_;
  Field<uint8_t, core::String> app_id_;
  Field<uint8_t, core::String> ctx_id_;
  core::Optional<uint32_t> session_id_;
  Field<uint8_t, core::String> file_name_;
  core::Optional<uint32_t> line_num_;
  Field<uint8_t, Field<uint8_t, core::String>> tags_;
};

class Payload {
  static constexpr std::uint8_t kTypeBoolOffset{4U};
  static constexpr std::uint8_t kTypeSignedOffset{5U};
  static constexpr std::uint8_t kTypeUnsignedOffset{6U};
  static constexpr std::uint8_t kTypeFloatOffset{7U};
  static constexpr std::uint8_t kTypeStringOffset{9U};
  class Argument {
    using ValueType = std::variant<bool, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, std::int8_t,
                                   std::int16_t, std::int32_t, std::int64_t, double, float, core::StringView>;

   public:
    template <typename Ty_>
    Argument(Ty_ value) {
      using T = std::decay_t<Ty_>;
      if constexpr (std::is_same_v<T, bool>) {
        type_info_ = 1U;
        type_info_ |= 1U << kTypeBoolOffset;
        data_payload_.resize(sizeof(T));
        std::memcpy(data_payload_.data(), &value, sizeof(T));
      } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
        if constexpr (std::is_same_v<T, std::int8_t>) {
          type_info_ = 1U;
        } else if constexpr (std::is_same_v<T, std::int16_t>) {
          type_info_ = 2U;
        } else if constexpr (std::is_same_v<T, std::int32_t>) {
          type_info_ = 3U;
        } else if constexpr (std::is_same_v<T, std::int64_t>) {
          type_info_ = 4U;
        } else {
          // TODO support 128bit signed integer
        }
        type_info_ |= 1U << kTypeSignedOffset;
        data_payload_.resize(sizeof(T));
        std::memcpy(data_payload_.data(), &value, sizeof(T));
      } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
        if constexpr (std::is_same_v<T, std::uint8_t>) {
          type_info_ = 1U;
        } else if constexpr (std::is_same_v<T, std::uint16_t>) {
          type_info_ = 2U;
        } else if constexpr (std::is_same_v<T, std::uint32_t>) {
          type_info_ = 3U;
        } else if constexpr (std::is_same_v<T, std::uint64_t>) {
          type_info_ = 4U;
        } else {
          // TODO support 128bit unsigned integer
        }
        type_info_ |= 1U << kTypeUnsignedOffset;
        data_payload_.resize(sizeof(T));
        std::memcpy(data_payload_.data(), &value, sizeof(T));
      } else if constexpr (std::is_floating_point_v<T>) {
        if constexpr (std::is_same_v<T, float>) {
          type_info_ = 3U;
        } else if constexpr (std::is_same_v<T, double>) {
          type_info_ = 4U;
        } else {
          // TODO support long double and half
        }
        type_info_ |= 1U << kTypeFloatOffset;
        data_payload_.resize(sizeof(T));
        std::memcpy(data_payload_.data(), &value, sizeof(T));
      } else if constexpr (std::is_same_v<T, const char*>) {
        type_info_ |= 1U << kTypeStringOffset;
        data_payload_.resize(strlen(value) + 1);
        std::memcpy(data_payload_.data(), value, strlen(value));
      }
    }

    ValueType GetValue() const;

   private:
    std::uint32_t type_info_{0U};
    core::Vector<core::Byte> data_payload_;
    ValueType value_;
  };

 public:
  template <typename T>
  void AddArgument(T&& arg) {
    arguments_.emplace_back(std::forward<T>(arg));
  }

  core::String ToString() const;

 private:
  core::Vector<Argument> arguments_;
};

class Message {
  struct ThisIsPrivateType;

 public:
  static std::shared_ptr<Message> VerboseModeLogMessage(LogLevel log_level, core::StringView ctx_id);

  Message(ThisIsPrivateType, BaseHeader&& base_header);

  template <typename T>
  void AddArgument(T&& arg) {
    payload_->AddArgument(std::forward<T>(arg));
  }

  const core::String& ToString() const;

 private:
  struct ThisIsPrivateType {};

  static std::shared_ptr<Message> Create(BaseHeader&& base_header);

 private:
  BaseHeader base_header_;
  core::Optional<ExtensionHeader> ext_header_;
  core::Optional<Payload> payload_;
  mutable core::Optional<core::String> text_;
  thread_local static std::int64_t thread_id_;
};
}  // namespace ara::log::dlt

#endif