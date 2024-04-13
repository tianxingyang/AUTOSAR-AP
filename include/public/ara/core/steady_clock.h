#ifndef VITO_AP_STEADY_CLOCK_H_
#define VITO_AP_STEADY_CLOCK_H_

#include <chrono>

namespace ara::core {
class SteadyClock final {
 public:
  /// @brief An arithmetic type representing the number of ticks in the clock's duration .
  using rep = std::int64_t;

  /// @brief A std::ratio type representing the tick period of the clock, in seconds .
  using period = std::nano;

  /// @brief std::chrono::duration<rep, period>
  using duration = std::chrono::duration<rep, period>;

  /// @brief std::chrono::time_point<ara::core::SteadyClock>
  using time_point = std::chrono::time_point<SteadyClock, duration>;

  /// @brief steady clock flag, always true
  static constexpr bool is_steady = true;

  /// @brief Return a time_point representing the current value of the clock.
  /// @return a time_point
  static time_point now() noexcept {
    return time_point{std::chrono::duration_cast<duration>(std::chrono::steady_clock::now().time_since_epoch())};
  }
};
}  // namespace ara::core

#endif