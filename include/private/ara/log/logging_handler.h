#ifndef VITO_AP_LOGGING_HANDLER_
#define VITO_AP_LOGGING_HANDLER_

namespace ara::log {

class LogStream;

class LoggingHandler {
 public:
  virtual ~LoggingHandler() = default;
  virtual void Emit(const LogStream&) = 0;
};

class ConsoleHandler final : public LoggingHandler {
 public:
  ~ConsoleHandler() override = default;
  void Emit(const LogStream&) override;
};

class FileHandler : public LoggingHandler {
 public:
  ~FileHandler() override = default;
  void Emit(const LogStream&) override;
};

class NetworkHandler : public LoggingHandler {
 public:
  ~NetworkHandler() override = default;
  void Emit(const LogStream&) override;
};

class BaseRotatingHandler : public FileHandler {
 public:
  ~BaseRotatingHandler() override = default;
  void Emit(const LogStream&) override;

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