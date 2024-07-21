#ifndef VITO_AP_SINGLETON_PATTERN_H_
#define VITO_AP_SINGLETON_PATTERN_H_

namespace ara::core {
template <typename T>
class Singleton {
 public:
  static T& Instance() {
    static T singleton;
    return singleton;
  }

  Singleton(const Singleton&) = delete;
  Singleton(Singleton&&) = default;
  Singleton& operator=(const Singleton&) = delete;
  Singleton& operator=(Singleton&&) = default;
  ~Singleton() = default;

 protected:
  Singleton() = default;
};
}  // namespace ara::core

#endif