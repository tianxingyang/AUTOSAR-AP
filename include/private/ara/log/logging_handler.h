#ifndef VITO_AP_LOGGING_HANDLER_
#define VITO_AP_LOGGING_HANDLER_

#include <memory>

namespace ara::log {

namespace dlt {
class Message;
}

class LoggingHandler {
 public:
  virtual ~LoggingHandler() = default;
  virtual void Emit(std::shared_ptr<dlt::Message> message) = 0;
};

class ConsoleHandler final : public LoggingHandler {
 public:
  ~ConsoleHandler() override = default;
  void Emit(std::shared_ptr<dlt::Message> message) override;
};

class FileHandler : public LoggingHandler {
 public:
  ~FileHandler() override = default;
  void Emit(std::shared_ptr<dlt::Message> message) override;
};

class NetworkHandler : public LoggingHandler {
 public:
  ~NetworkHandler() override = default;
  void Emit(std::shared_ptr<dlt::Message> message) override;
};

class BaseRotatingHandler : public FileHandler {
 public:
  ~BaseRotatingHandler() override = default;
  void Emit(std::shared_ptr<dlt::Message> message) override;

 protected:
  virtual bool ShouldRollover() = 0;
  virtual void DoRollover() = 0;
};

class RotatingFileHandler final : public BaseRotatingHandler {
 public:
  ~RotatingFileHandler() override = default;

 protected:
  bool ShouldRollover() override;
  void DoRollover() override;
};

class TimedRotatingFileHandler final : public BaseRotatingHandler {
 public:
  ~TimedRotatingFileHandler() override = default;

 protected:
  bool ShouldRollover() override;
  void DoRollover() override;
};
}  // namespace ara::log

#endif