#include "ara/core/initialization.h"
#include "ara/log/logger.h"

int main() {
  ara::core::Initialize();
  auto const logger{ara::log::CreateLogger("em", "em", ara::log::LogLevel::kInfo)};
  logger.LogInfo() << "run!" << true;
  return 0;
}