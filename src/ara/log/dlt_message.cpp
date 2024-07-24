#include "ara/log/dlt_message.h"

#include <chrono>

#include "ara/core/string_view.h"
#include "ara/log/common.h"
#include "ara/log/dlt_message.h"
#include "ara/log/log_config.h"
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
    default:
      return "UNKNOWN";
  }
}
}  // namespace

namespace ara::log::dlt {
thread_local std::thread::id Message::thread_id_{std::this_thread::get_id()};
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

const core::String BaseHeader::GetTimeStr() const {
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

const core::Vector<Payload::Argument>& Payload::Arguments() const { return arguments_; }

core::String Payload::Argument::ToString() const {
  if (type_info_ & (1 << kTypeBoolOffset)) {
    return std::to_integer<bool>(data_payload_[0]) ? "true" : "false";
  } else if (type_info_ & (1 << kTypeSignedOffset)) {
  } else if (type_info_ & (1 << kTypeUnsignedOffset)) {
  } else if (type_info_ & (1 << kTypeFloatOffset)) {
  } else if (type_info_ & (1 << kTypeStringOffset)) {
  } else {
  }
  return "";
}

std::shared_ptr<Message> Message::VerboseModeLogMessage(LogLevel log_level, core::StringView ctx_id) {
  auto msg_ptr = Message::Create(BaseHeader::VerboseModeLogBaseHeader(HeaderType::VerboseMode(), log_level));
  msg_ptr->ext_header_ = ExtensionHeader{};
  msg_ptr->ext_header_->SetEcuId(LogConfig::Instance().EcuId());
  msg_ptr->ext_header_->SetAppId(LogConfig::Instance().AppId());
  msg_ptr->ext_header_->SetCtxId(ctx_id);
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

  for (auto& arg : payload_->Arguments()) {
    fmt::format_to(std::back_inserter(*text_), "{} ", arg.ToString());
  }
  return text_.value();
}

}  // namespace ara::log::dlt