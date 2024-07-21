#include "ara/log/dlt_message.h"

#include <chrono>

#include "ara/log/dlt_message.h"

namespace ara::log::dlt {
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

MessageInfo::MessageInfo(MessageType message_type, std::uint8_t const message_type_info) {
  value_[1] = (static_cast<uint8_t>(message_type) >> 1) & 1;
  value_[2] = (static_cast<uint8_t>(message_type) >> 2) & 1;
  value_[3] = (static_cast<uint8_t>(message_type) >> 3) & 1;

  value_[4] = (message_type_info >> 4) & 1;
  value_[5] = (message_type_info >> 5) & 1;
  value_[6] = (message_type_info >> 6) & 1;
  value_[7] = (message_type_info >> 7) & 1;
}

Timestamp::Timestamp() {
  const auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
  nanoseconds_ = now_ns.count() % 1'000'000'000;
  seconds_ = now_ns.count() / 1'000'000'000;

  // TODO set mask
}

BaseHeader BaseHeader::VerboseModeLogBaseHeader(HeaderType&& header_type, LogLevel log_level) {
  BaseHeader base_header{std::move(header_type)};
  base_header.message_info_ = MessageInfo::LogMessage(log_level);
  return base_header;
}

BaseHeader::BaseHeader(HeaderType&& header_type) : header_type_{header_type} {}

std::shared_ptr<Message> Message::VerboseModeLogMessage(LogLevel log_level) {
  return std::make_shared<Message>(BaseHeader::VerboseModeLogBaseHeader(HeaderType::VerboseMode(), log_level));
}

Message::Message(BaseHeader&& base_header) : base_header_{base_header} {}
}  // namespace ara::log::dlt