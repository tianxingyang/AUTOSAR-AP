#include "ara/core/initialization.h"
#include "ara/log/logger.h"

void Run() {
  const auto logger{ara::log::CreateLogger("em", "em", ara::log::LogLevel::kVerbose)};
  for (int i{0}; i < 100; ++i) {
    logger.LogVerbose() << true;
    logger.LogDebug() << false;
    logger.LogInfo() << true;
    logger.LogWarn() << false;
    logger.LogError() << true;
    logger.LogFatal() << false;
  }
}

int main() {
  if (const auto result{ara::core::Initialize()}; !result) {
    return 1;
  }

  Run();

  ara::core::Deinitialize();
  return 0;
}