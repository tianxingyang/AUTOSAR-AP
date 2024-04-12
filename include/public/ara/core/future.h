#ifndef VITO_AP_FUTURE_H_
#define VITO_AP_FUTURE_H_

#include <chrono>
#include <cstdint>
#include <future>

#include "ara/core/error_code.h"
#include "ara/core/future_error_domain.h"
#include "ara/core/result.h"

namespace ara::core {

namespace internal {
class State final {
 public:
  using Callback = std::function<void()>;
  State() = default;

  void FireCallback() {
    decltype(cbk_) callback;
    {
      std::unique_lock const lock{mtx_};
      if (!cbk_) {
        return;
      }

      callback = cbk_;
      cbk_ = nullptr;
    }

    callback();
  }

  void SetCallback(Callback callback) {
    std::unique_lock const lock{mtx_};
    cbk_ = std::move(callback);
  }

 private:
  std::mutex mtx_;
  Callback cbk_;
};

using StatePtr = std::shared_ptr<State>;
}  // namespace internal

template <typename T, typename E>
class Future;

/// @brief ara::core specific variant of std::promise class
/// @tparam T the type of value
/// @tparam E the type of error
template <typename T, typename E = ErrorCode>
class Promise final {
 public:
  /// @brief Default constructor.
  /// This function shall behave the same as the corresponding std::promise function.
  Promise() = default;

  /// @brief Move constructor.
  /// This function shall behave the same as the corresponding std::promise function.
  /// @param other the other instance
  Promise(Promise&& other) noexcept = default;

  /// @brief Copy constructor shall be disabled.
  Promise(const Promise&) = delete;

  /// @brief Destructor for Promise objects.
  /// This function shall behave the same as the corresponding std::promise function.
  ~Promise() noexcept = default;

  /// @brief Move assignment.
  /// This function shall behave the same as the corresponding std::promise function.
  /// @param other the other instance
  /// @return *this
  Promise& operator=(Promise&& other) noexcept = default;

  /// @brief Copy assignment operator shall be disabled.
  Promise& operator=(const Promise&) noexcept = delete;

  /// @brief Swap the contents of this instance with another one's.
  /// This function shall behave the same as the corresponding std::promise function.
  /// @param other the other instance
  void swap(Promise& other) noexcept { promise_.swap(other.promise_); }

  /// @brief Return the associated Future.
  /// The returned Future is set as soon as this Promise receives the result or an error. This method must only be
  /// called once as it is not allowed to have multiple Futures per Promise.
  /// @return a Future
  Future<T, E> get_future() { return Future<T, E>{promise_.get_future()}; }

  /// @brief Copy a value into the shared state and make the state ready.
  /// This function shall behave the same as the corresponding std::promise function.
  /// @param value the value to store
  void set_value(const T& value) { promise_.set_value(Result<T, E>::FromValue(value)); }

  /// @brief Move a value into the shared state and make the state ready.
  /// This function shall behave the same as the corresponding std::promise function.
  /// @param value the value to store
  void set_value(T&& value) { promise_.set_value(Result<T, E>::FromValue(std::move(value))); }

  /// @brief Move an error into the shared state and make the state ready.
  /// @param error the error to store.
  void SetError(E&& error) { promise_.set_value(Result<T, E>::FromError(std::move(error))); }

  /// @brief Copy an error into the shared state and make the state ready.
  /// @param error the error to store
  void SetError(const E& error) { promise_.set_value(Result<T, E>::FromError(error)); }

  /// @brief Copy a Result into the shared state and make the state ready.
  /// @param result the result to store
  void SetResult(const Result<T, E>& result) {
    if (result) {
      set_value(result.Value());
    } else {
      SetError(result.Error());
    }
  }

  /// @brief Move a Result into the shared state and make the state ready.
  /// @param result the result to store
  void SetResult(Result<T, E>&& result) {
    if (result) {
      set_value(std::move(result).Value());
    } else {
      SetError(std::move(result).Error());
    }
  }

 private:
  std::promise<Result<T, E>> promise_;
};

/// @brief Specifies the state of a Future as returned by wait_for() and wait_until().
/// These definitions are equivalent to the ones from std::future_status. However, no item equivalent to
/// std::future_status::deferred is available here.
/// The numerical values of the enum items are implementation-defined.
// ReSharper disable once CppInconsistentNaming
enum class future_status : std::uint8_t {
  /// @brief the shared state is ready
  // ReSharper disable once CppInconsistentNaming
  ready,
  /// @brief the shared state did not become ready before the specified timeout has passed
  // ReSharper disable once CppInconsistentNaming
  timeout,
};

/// @brief Provides ara::core specific Future operations to collect the results of an asynchronous call.
/// @tparam T the type of values
/// @tparam E the type of errors
template <typename T, typename E = ErrorCode>
class Future final {
  friend class Promise<T, E>;
  explicit Future(std::future<Result<T, E>>&& future) : future_{std::move(future)} {}

 public:
  /// @brief Default constructor.
  /// This function shall behave the same as the corresponding std::future function.
  Future() noexcept = default;

  /// @brief Copy constructor shall be disabled.
  Future(const Future&) = delete;

  /// @brief Move construct from another instance.
  /// This function shall behave the same as the corresponding std::future function.
  /// @param other the other instance
  Future(Future&& other) noexcept { std::swap(other.future_, future_); }

  /// @brief Destructor for Future objects.
  /// This function shall behave the same as the corresponding std::future function.
  ~Future() noexcept = default;

  /// @brief Copy assignment operator shall be disabled.
  Future& operator=(const Future&) = delete;

  /// @brief Move assign from another instance.
  /// This function shall behave the same as the corresponding std::future function.
  /// @param other the other instance
  /// @return *this
  Future& operator=(Future&& other) noexcept {
    future_ = std::move(other.future_);
    return *this;
  }

  /// @brief Get the value.
  /// This function shall behave the same as the corresponding std::future function.
  /// This function does not participate in overload resolution when the compiler toolchain does not support C++
  /// exceptions.
  /// @return value of type T
  T get() { return GetResult().ValueOrThrow(); }

  /// @brief Get the result.
  /// @return a Result with either a value or an error
  Result<T, E> GetResult() noexcept {
    using R = Result<T, E>;
    try {
      return R{future_.get()};
    } catch (const std::future_error& ex) {
      switch (const auto error_code{static_cast<std::future_errc>(ex.code().value())}) {
        case std::future_errc::broken_promise:
          return R::FromError(future_errc::broken_promise);
        case std::future_errc::future_already_retrieved:
          return R::FromError(future_errc::future_already_retrieved);
        case std::future_errc::no_state:
          return R::FromError(future_errc::no_state);
        case std::future_errc::promise_already_satisfied:
          return R::FromError(future_errc::promise_already_satisfied);
        default:
          return R::FromError(future_errc::kInvalidArgument);
      }
    }
  }

  /// @brief Checks if the Future is valid, i.e. if it has a shared state.
  /// This function shall behave the same as the corresponding std::future function.
  /// @return true if the Future is usable, false otherwise
  bool valid() const noexcept { return future_.valid(); }

  /// @brief Wait for a value or an error to be available.
  /// This function shall behave the same as the corresponding std::future function.
  void wait() const { return future_.wait(); }

  /// @brief Wait for the given period, or until a value or an error is available.
  /// @tparam Rep
  /// @tparam Period
  /// @param timeout_duration maximal duration to wait for
  /// @return status that indicates whether the timeout hit or if a value is available
  template <typename Rep, typename Period>
  future_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const {
    return static_cast<future_status>(future_.wait_for(timeout_duration));
  }

  /// @brief Wait until the given time, or until a value or an error is available.
  /// This function shall behave the same as the corresponding std::future function.
  /// @tparam Clock
  /// @tparam Duration
  /// @param deadline latest point in time to wait
  /// @return status that indicates whether the time was reached or if a value is available
  template <typename Clock, typename Duration>
  future_status wait_until(const std::chrono::time_point<Clock, Duration>& deadline) const {
    return static_cast<future_status>(future_.wait_until(deadline));
  }

  /// @brief Register a callable that gets called when the Future becomes ready.
  /// When func is called, it is guaranteed that get() and GetResult() will not block.
  /// func may be called in the context of this call or in the context of Promise::set_value() or Promise::SetError() or
  /// somewhere else.
  /// The return type of then depends on the return type of func (aka continuation). Let U be the return
  /// type of the continuation (i.e. a type equivalent to std::result_of_t<std::decay_t<F>(Future<T,E>)>).
  /// If U is Future<T2,E2> for some types T2, E2, then the return type of then() is Future<T2,E2>. This is known as
  /// implicit Future unwrapping.
  /// If U is Result<T2,E2> for some types T2, E2, then the return type of then() is Future<T2,E2>. This is known as
  /// implicit Result unwrapping.
  /// Otherwise, it is Future<U,E>.
  /// @tparam F the type of the func argument
  /// @param func a callable to register
  /// @return a new Future instance for the result of the continuation.
  // template <typename F>
  // auto then(F&& func) -> std::invoke_result_t<std::decay_t<F>, Future> {
  //   auto sharedPromise = std::make_shared<Promise<T, E>>();
  //   auto sharedState = state_;
  //   auto sharedFuture = std::make_shared<Future>(std::move(*this));

  //  sharedState->SetCallback([sharedPromise, func = std::decay_t<F>(std::forward<F>(func)), sharedFuture]() {
  //    sharedPromise->set_value(func(std::move(*sharedFuture.get())));
  //  });

  //  if (sharedFuture->is_ready()) {
  //    sharedState->FireCallback();
  //  }

  //  return sharedPromise->get_future();
  //}

  /// @brief Register a callable that gets called when the Future becomes ready
  /// When func is called, it is guaranteed that get() and GetResult() will not block.
  /// func is called in the context of the provided execution context executor.
  /// The return type of depends on the return type of func (aka continuation).
  /// Let U be the return type of the continuation (i.e. a type equivalent to
  /// std::result_of_t<std::decay_t<F>(Future<T,E>)>). If U is Future<T2,E2> for some types T2, E2, then the return type
  /// of then() is Future<T2,E2>. This is known as implicit Future unwrapping. If U is Result<T2,E2> for some types T2,
  /// E2, then the return type of then() is Future<T2,E2>. This is known as implicit Result unwrapping. Otherwise it is
  /// Future<U,E>.
  /// @tparam F the type of the func argument
  /// @tparam ExecutorT the type of the executor argument
  /// @param func a callable to register
  /// @param executor the execution context in which to execute the Callable func
  /// @return a new Future instance for the result of the continuation
  // template <typename F, typename ExecutorT>
  // auto then(F&& func, ExecutorT&& executor);

  /// @brief Return whether the asynchronous operation has finished.
  /// If this function returns true, get(), GetResult() and the wait calls are guaranteed not to block. The behavior of
  /// this function is undefined if valid() returns false.
  /// @return true if the Future contains a value or an error, false otherwise.
  bool is_ready() const { return wait_for(std::chrono::seconds::zero()) == future_status::ready; }

 private:
  std::future<Result<T, E>> future_;
  // internal::StatePtr state_;
};

/// @brief Specialization of class Future for "void" values.
/// @tparam E the type of error
template <typename E>
class Future<void, E> final {
 public:
  /// @brief Default constructor.
  /// This function shall behave the same as the corresponding std::future function.
  Future() noexcept = default;

  /// @brief Copy constructor shall be disabled.
  /// @param
  Future(const Future&) = delete;

  /// @brief Move construct from another instance.
  /// This function shall behave the same as the corresponding std::future function.
  /// @param other the other instance
  Future(Future&& other) noexcept = default;

  /// @brief Destructor for Future objects.
  /// This function shall behave the same as the corresponding std::future function.
  ~Future() noexcept = default;

  /// @brief Copy assignment operator shall be disabled.
  /// @param
  /// @return
  Future& operator=(const Future&) = delete;

  /// @brief Move assign from another instance.
  /// This function shall behave the same as the corresponding std::future function.
  /// @param other the other instance
  /// @return *this
  Future& operator=(Future&& other) noexcept = default;

  /// @brief Get the value.
  /// This function shall behave the same as the corresponding std::future function.
  /// This function does not participate in overload resolution when the compiler toolchain does not support C++
  /// exceptions.
  void get() { GetResult().ValueOrThrow(); }

  /// @brief Get the result.
  /// Similar to get(), this call blocks until the value or an error is available. However, this call will never throw
  /// an exception.
  /// @return a Result with either a value or an error
  Result<void, E> GetResult() noexcept {
    using R = Result<void, E>;
    try {
      future_.get();
      return R{};
    } catch (const std::future_error& ex) {
      switch (const auto error_code{static_cast<std::future_errc>(ex.code().value())}) {
        case std::future_errc::broken_promise:
          return R::FromError(future_errc::broken_promise);
        case std::future_errc::future_already_retrieved:
          return R::FromError(future_errc::future_already_retrieved);
        case std::future_errc::no_state:
          return R::FromError(future_errc::no_state);
        case std::future_errc::promise_already_satisfied:
          return R::FromError(future_errc::promise_already_satisfied);
        default:
          return R::FromError(future_errc::kInvalidArgument);
      }
    }
  }

  /// @brief Checks if the Future is valid, i.e. if it has a shared state.
  /// This function shall behave the same as the corresponding std::future function.
  /// @return true if the Future is usable, false otherwise
  bool valid() const noexcept { return future_.valid(); }

  /// @brief Wait for a value or an error to be available.
  /// This function shall behave the same as the corresponding std::future function.
  void wait() const { future_.wait(); }

  /// @brief Wait for the given period, or until a value or an error is available.
  /// This function shall behave the same as the corresponding std::future function.
  /// @tparam Rep
  /// @tparam Period
  /// @param timeoutDuration maximal duration to wait for
  /// @return status that indicates whether the timeout hit or if a value is available
  template <typename Rep, typename Period>
  future_status wait_for(const std::chrono::duration<Rep, Period>& timeoutDuration) const {
    return future_.wait_for(timeoutDuration);
  }

  /// @brief Wait until the given time, or until a value or an error is available.
  /// This function shall behave the same as the corresponding std::future function.
  /// @tparam Clock
  /// @tparam Duration
  /// @param deadline latest point in time to wait
  /// @return status that indicates whether the time was reached or if a value is available
  template <typename Clock, typename Duration>
  future_status wait_until(const std::chrono::time_point<Clock, Duration>& deadline) const {
    return future_.wait_until(deadline);
  }

  /// @brief Return whether the asynchronous operation has finished.
  /// If this function returns true, get(), GetResult() and the wait calls are guaranteed not to block.
  /// The behavior of this function is undefined if valid() returns false.
  /// @return true if the Future contains a value or an error, false otherwise
  bool is_ready() const { return wait_for(std::chrono::seconds::zero()) == future_status::ready; }

 private:
  std::future<Result<void, E>> future_;
};
}  // namespace ara::core

#endif
