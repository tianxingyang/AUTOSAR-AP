#include <thread>

#include "ara/core/initialization.h"
#include "ara/log/log_fwd.h"

bool exit_flag{false};

void Run() {
  const auto logger{ara::log::CreateLogger("em", "em", ara::log::LogLevel::kVerbose)};

  while (!exit_flag) {
    logger.LogInfo() << "Hello, World!";
  }
}

int SignalHandler(int signal) {
  exit_flag = true;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  ara::core::Deinitialize();
  return signal;
}

int main() {
  if (const auto result{ara::core::Initialize()}; !result) {
    return 1;
  }

  Run();

  return 0;
}