#include "ara/log/dlt_message.h"

#include <syscall.h>

#include <cassert>
#include <chrono>

#include "ara/core/string_view.h"
#include "ara/log/common.h"
#include "ara/log/log_config.h"
#include "fmt/chrono.h"
#include "fmt/core.h"
#include "fmt/std.h"

namespace {
constexpr ara::core::StringView kTextFormat{"{time}|{ecu_id}|{app_id}|{ctx_id}|{thread_id}|{log_level}|"};

ara::core::StringView LogLevelToString(ara::log::LogLevel log_level) {
  switch (log_level) {
    case ara::log::LogLevel::kFatal:
      return "FATAL";
    case ara::log::LogLevel::kError:
      return "ERROR";
    case ara::log::LogLevel::kWarn:
      return "WARN";
    case ara::log::LogLevel::kInfo:
      return "INFO";
    case ara::log::LogLevel::kDebug:
      return "DEBUG";
    case ara::log::LogLevel::kVerbose:
      return "VERBOSE";
    case ara::log::LogLevel::kOff:
      return "OFF";
    default:
      return "UNKNOWN";
  }
}

std::int64_t GetSignedInteger(std::uint32_t type_info, const ara::core::Vector<ara::core::Byte>& bytes) {
  if ((type_info & 0xF) == 0x01) {
    return *reinterpret_cast<const std::int8_t*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x02) {
    return *reinterpret_cast<const std::int16_t*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x03) {
    return *reinterpret_cast<const std::int32_t*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x04) {
    return *reinterpret_cast<const std::int64_t*>(bytes.data());
  }

  assert(false);
}

std::uint64_t GetUnsignedInteger(std::uint32_t type_info, const ara::core::Vector<ara::core::Byte>& bytes) {
  if ((type_info & 0xF) == 0x01) {
    return *reinterpret_cast<const std::uint8_t*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x02) {
    return *reinterpret_cast<const std::uint16_t*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x03) {
    return *reinterpret_cast<const std::uint32_t*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x04) {
    return *reinterpret_cast<const std::uint64_t*>(bytes.data());
  }

  assert(false);
}

double GetFloat(std::uint32_t type_info, const ara::core::Vector<ara::core::Byte>& bytes) {
  if ((type_info & 0xF) == 0x03) {
    return *reinterpret_cast<const float*>(bytes.data());
  }
  if ((type_info & 0xF) == 0x04) {
    return *reinterpret_cast<const double*>(bytes.data());
  }

  assert(false);
}
}  // namespace

namespace ara::log::dlt {
thread_local std::int64_t Message::thread_id_{syscall(SYS_gettid)};
std::atomic_uint8_t BaseHeader::message_counter_{0};
constexpr std::uint8_t kVersionNumber{2};

HeaderType::HeaderType() {
  value_[5] = (kVersionNumber >> 0) & 1;
  value_[6] = (kVersionNumber >> 1) & 1;
  value_[7] = (kVersionNumber >> 2) & 1;
}

HeaderType HeaderType::VerboseMode() {
  HeaderType header_type;
  header_type.SetContentInfo(Cnti::kVerboseModeDataMessage);
  header_type.SetWithEcuId(true);
  header_type.SetWithAppAndCtxId(true);
  header_type.SetWithSessionId(false);
  header_type.SetWithSourceFileNameAndLine(false);
  header_type.SetWithTags(false);
  header_type.SetWithPrivacyLevel(false);
  header_type.SetWithSegmentation(false);

  return header_type;
}

void HeaderType::SetContentInfo(Cnti cnti) {
  value_[0] = (static_cast<std::uint8_t>(cnti) >> 0) & 1;
  value_[1] = (static_cast<std::uint8_t>(cnti) >> 1) & 1;
}

HeaderType::Cnti HeaderType::GetContentInfo() const {
  std::uint8_t cnti{0};
  cnti |= value_[0] << 0;
  cnti |= value_[1] << 1;

  return static_cast<Cnti>(cnti);
}

void HeaderType::SetWithEcuId(const bool with) { value_[2] = with; }

bool HeaderType::GetWithEcuId() const { return value_[2]; }

void HeaderType::SetWithAppAndCtxId(const bool with) { value_[3] = with; }

bool HeaderType::GetWithAppAndCtxId() const { return value_[3]; }

void HeaderType::SetWithSessionId(const bool with) { value_[4] = with; }

bool HeaderType::GetWithSessionId() const { return value_[4]; }

void HeaderType::SetWithSourceFileNameAndLine(const bool with) { value_[8] = with; }

bool HeaderType::GetWithSourceFileNameAndLine() const { return value_[8]; }

void HeaderType::SetWithTags(const bool with) { value_[9] = with; }

bool HeaderType::GetWithTags() const { return value_[9]; }

void HeaderType::SetWithPrivacyLevel(const bool with) { value_[10] = with; }

bool HeaderType::GetWithPrivacyLevel() const { return value_[10]; }

void HeaderType::SetWithSegmentation(const bool with) { value_[11] = with; }

bool HeaderType::GetWithSegmentation() const { return value_[11]; }

MessageInfo MessageInfo::LogMessage(LogLevel log_level) {
  return MessageInfo{MessageType::kLog, static_cast<std::uint8_t>(log_level)};
}

MessageInfo MessageInfo::TraceMessage(TraceMessageInfo trace_info) {
  return MessageInfo{MessageType::kTrace, static_cast<std::uint8_t>(trace_info)};
}

MessageInfo MessageInfo::NetworkMessage(NetworkMessageInfo network_info) {
  return MessageInfo{MessageType::kNetwork, static_cast<std::uint8_t>(network_info)};
}

LogLevel MessageInfo::GetLogLevel() const {
  return static_cast<LogLevel>(value_[7] << 3 | value_[6] << 2 | value_[5] << 1 | value_[4]);
}

MessageInfo::MessageInfo(MessageType message_type, std::uint8_t const message_type_info) {
  value_[1] = (static_cast<uint8_t>(message_type) >> 0) & 1;
  value_[2] = (static_cast<uint8_t>(message_type) >> 1) & 1;
  value_[3] = (static_cast<uint8_t>(message_type) >> 2) & 1;

  value_[4] = (message_type_info >> 0) & 1;
  value_[5] = (message_type_info >> 1) & 1;
  value_[6] = (message_type_info >> 2) & 1;
  value_[7] = (message_type_info >> 3) & 1;
}

Timestamp::Timestamp() {
  const auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
  nanoseconds_ = now_ns.count() % 1'000'000'000;
  seconds_ = now_ns.count() / 1'000'000'000;

  // TODO set mask
}

const core::String Timestamp::ToString() const {
  return fmt::format("{:%Y-%m-%d %H:%M:%S}.{:0>9}", fmt::localtime(seconds_), nanoseconds_);
}

BaseHeader BaseHeader::VerboseModeLogBaseHeader(HeaderType&& header_type, LogLevel log_level) {
  BaseHeader base_header{std::move(header_type)};
  base_header.message_info_ = MessageInfo::LogMessage(log_level);
  base_header.timestamp_ = Timestamp{};
  return base_header;
}

LogLevel BaseHeader::GetLogLevel() const {
  if (!message_info_) {
    return LogLevel::kOff;
  }

  return message_info_->GetLogLevel();
}

core::String BaseHeader::GetTimeStr() const {
  if (!timestamp_) {
    return {};
  }

  return timestamp_->ToString();
}

BaseHeader::BaseHeader(HeaderType&& header_type) : header_type_{header_type} {}

void ExtensionHeader::SetEcuId(core::StringView ecu_id) {
  ecu_id_.desc = ecu_id.size();
  ecu_id_.value = ecu_id;
}

core::StringView ExtensionHeader::EcuId() const {
  if (!ecu_id_.value) {
    return "";
  }
  return *ecu_id_.value;
}

void ExtensionHeader::SetAppId(core::StringView app_id) {
  app_id_.desc = app_id.size();
  app_id_.value = app_id;
}

core::StringView ExtensionHeader::AppId() const {
  if (!app_id_.value) {
    return "";
  }
  return *app_id_.value;
}

void ExtensionHeader::SetCtxId(core::StringView ctx_id) {
  ctx_id_.desc = ctx_id.size();
  ctx_id_.value = ctx_id;
}

core::StringView ExtensionHeader::CtxId() const {
  if (!ctx_id_.value) {
    return "";
  }
  return *ctx_id_.value;
}

core::String Payload::ToString() const {
  core::String str;
  for (auto& arg : arguments_) {
    std::visit([&str](auto value) { fmt::format_to(std::back_inserter(str), "{} ", value); }, arg.GetValue());
  }
  return str;
}

Payload::Argument::ValueType Payload::Argument::GetValue() const {
  if (type_info_ & (1 << kTypeBoolOffset)) {
    return std::to_integer<bool>(data_payload_[0]);
  }
  if (type_info_ & (1 << kTypeSignedOffset)) {
    return GetSignedInteger(type_info_, data_payload_);
  }
  if (type_info_ & (1 << kTypeUnsignedOffset)) {
    return GetUnsignedInteger(type_info_, data_payload_);
  }
  if (type_info_ & (1 << kTypeFloatOffset)) {
    return GetFloat(type_info_, data_payload_);
  }
  if (type_info_ & (1 << kTypeStringOffset)) {
    return core::StringView{reinterpret_cast<const char*>(data_payload_.data()), data_payload_.size()};
  }

  assert(false);
}

std::shared_ptr<Message> Message::VerboseModeLogMessage(LogLevel log_level, core::StringView ctx_id) {
  auto msg_ptr = Create(BaseHeader::VerboseModeLogBaseHeader(HeaderType::VerboseMode(), log_level));
  ExtensionHeader ext_header{};
  ext_header.SetEcuId(LogConfig::Instance().EcuId());
  ext_header.SetAppId(LogConfig::Instance().AppId());
  ext_header.SetCtxId(ctx_id);
  msg_ptr->ext_header_ = std::move(ext_header);
  msg_ptr->payload_ = Payload{};
  return msg_ptr;
}

Message::Message(ThisIsPrivateType, BaseHeader&& base_header) : base_header_{base_header} {}

std::shared_ptr<Message> Message::Create(BaseHeader&& base_header) {
  return std::make_shared<Message>(ThisIsPrivateType{}, std::move(base_header));
}

const core::String& Message::ToString() const {
  if (text_) {
    return text_.value();
  }

  text_ =
      fmt::format(kTextFormat, fmt::arg("time", base_header_.GetTimeStr()),
                  fmt::arg("ecu_id", ext_header_ ? ext_header_->EcuId() : "UNKNOWN"),
                  fmt::arg("app_id", ext_header_ ? ext_header_->AppId() : "UNKNOWN"),
                  fmt::arg("ctx_id", ext_header_ ? ext_header_->CtxId() : "UNKNOWN"), fmt::arg("thread_id", thread_id_),
                  fmt::arg("log_level", LogLevelToString(base_header_.GetLogLevel())));

  if (!payload_) {
    return text_.value();
  }

  fmt::format_to(std::back_inserter(*text_), "{} ", payload_->ToString());
  return text_.value();
}

}  // namespace ara::log::dlt