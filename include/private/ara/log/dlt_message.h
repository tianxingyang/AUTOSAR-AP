#ifndef VITO_AP_DLT_MESSAGE_H_
#define VITO_AP_DLT_MESSAGE_H_

#include <bitset>

#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"

namespace ara::log::dlt {
constexpr std::uint8_t kHtyp2Len{32};

class HeaderType {
 public:
  HeaderType();

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
  MessageInfo(MessageType message_type, std::uint8_t message_type_info);

 private:
  std::bitset<kMsinLen> value_;
};

class Timestamp {
 public:
  Timestamp();

 private:
  std::uint32_t nanoseconds_;
  std::uint64_t seconds_;
};

class BaseHeader {
 private:
  HeaderType header_type_;
  std::uint8_t message_counter_{0};
  std::uint16_t message_length_{0};
  core::Optional<MessageInfo> message_info_;
  core::Optional<std::uint8_t> number_of_arguments_;
  core::Optional<Timestamp> timestamp_;
  core::Optional<std::uint32_t> msid_;
};

class ExtensionHeader {
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
 private:
  class Argument {
   private:
    uint32_t type_info_{0U};
    core::Vector<core::Byte> data_payload_;
  };

  core::Vector<Argument> arguments_;
};

class Message {
 private:
  BaseHeader base_header_;
  core::Optional<ExtensionHeader> ext_header_;
  core::Optional<Payload> payload_;
};
}  // namespace ara::log::dlt

#endif