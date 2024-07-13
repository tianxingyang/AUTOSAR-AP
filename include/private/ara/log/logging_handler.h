#ifndef VITO_AP_LOGGING_HANDLER_
#define VITO_AP_LOGGING_HANDLER_

#include "ara/log/common.h"

namespace ara::log {
class ConsoleHandler final : public LoggingHandler {
 public:
  ~ConsoleHandler() override = default;
  void Emit() override;
};

class FileHandler : public LoggingHandler {
 public:
  ~FileHandler() override = default;
  void Emit() override;
};

class NetworkHandler : public LoggingHandler {
 public:
  ~NetworkHandler() override = default;
  void Emit() override;
};

class BaseRotatingHandler : public FileHandler {
 public:
  ~BaseRotatingHandler() override = default;
  void Emit() override;

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