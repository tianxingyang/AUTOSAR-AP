#include "ara/core/initialization.h"
#include "ara/log/logger.h"

void Run() {
  const auto logger{ara::log::CreateLogger("em", "em", ara::log::LogLevel::kInfo)};
  logger.LogInfo() << "run!" << true;
}

int main() {
  if (const auto result{ara::core::Initialize()}; !result) {
    return 1;
  }

  Run();

  ara::core::Deinitialize();
  return 0;
}