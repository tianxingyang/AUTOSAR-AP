#ifndef VITO_AP_RESULT_H_
#define VITO_AP_RESULT_H_

#include <functional>
#include <type_traits>
#include <variant>

#include "ara/core/error_code.h"
#include "ara/core/exception.h"
#include "ara/core/optional.h"

namespace ara::core {

class NoValueException : public Exception {};

/// @brief This class is a type that contains either a value or an error.
/// @tparam T the type of value
/// @tparam E the type of error
template <typename T, typename E = ErrorCode>
class Result final {
 public:
  /// @brief Type alias for the type T of values.
  using value_type = T;
  /// @brief Type alias for the type E of errors.
  using error_type = E;
  /// @brief Construct a new Result from the specified value (given as lvalue).
  /// @param t the value to put into the Result
  Result(const T& t) : data_{t} {}

  /// @brief Construct a new Result from the specified value (given as rvalue).
  /// @param t the value to put into the Result
  Result(T&& t) : data_{t} {}

  /// @brief Construct a new Result from the specified error (given as lvalue).
  /// @param e the error to put into the Result
  Result(const E& e) : data_{e} {}

  /// @brief Construct a new Result from the specified error (given as rvalue).
  /// @param e the error to put into the Result
  Result(E&& e) : data_{std::move(e)} {}

  /// @brief Copy-construct a new Result from another instance.
  /// @param other the other instance
  Result(const Result& other) : data_{other.data_} {}

  /// @brief Move-construct a new Result from another instance.
  /// @param other the other instance
  Result(Result&& other) noexcept(std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_constructible_v<E>)
      : data_{std::move(other.data_)} {}

  /// @brief Destructor.
  /// This destructor is trivial if std::is_trivially_destructible<T>::value && std::is_trivially_destructible<E>::value
  /// is true.
  ~Result() noexcept = default;

  /// @brief Build a new Result from the specified value (given as lvalue).
  /// @param t the value to put into the Result
  /// @return a Result that contains the value t
  static Result FromValue(const T& t) { return Result{t}; }

  /// @brief Build a new Result from the specified value (given as rvalue).
  /// @param t the value to put into the Result
  /// @return a Result that contains the value t
  static Result FromValue(T&& t) { return Result{t}; }

  /// @brief Build a new Result from a value that is constructed in-place from the given arguments.
  /// This function shall not participate in overload resolution unless : std::is_constructible<T, Args&&...>::value is
  /// true, and the first type of the expanded parameter pack is not T, and the first type of the expanded parameter
  /// pack is not a specialization of Result
  /// @tparam Args the types of arguments given to this function
  /// @param args the arguments used for constructing the value
  /// @return Result that contains a value
  template <typename... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>, bool> = true>
  static Result FromValue(Args&&... args) {
    return Result{T{std::forward<Args>(args)...}};
  }

  /// @brief Build a new Result from the specified error (given as lvalue).
  /// @param e the error to put into the Result
  /// @return a Result that contains the error e
  static Result FromError(const E& e) { return Result{e}; }

  /// @brief Build a new Result from the specified error (given as rvalue).
  /// @param e the error to put into the Result
  /// @return a Result that contains the error e
  static Result FromError(E&& e) { return Result{e}; }

  /// @brief Build a new Result from an error that is constructed in-place from the given arguments.
  /// This function shall not participate in overload resolution unless : std::is_constructible<E, Args&&...>::value is
  /// true, and the first type of the expanded parameter pack is not E, and the first type of the expanded parameter
  /// pack is not a specialization of Result
  /// @tparam Args the types of arguments given to this function
  /// @param args the arguments used for constructing the error
  /// @return a Result that contains an error
  template <typename... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>, bool> = true>
  static Result FromError(Args&&... args) {
    return Result{E{std::forward<Args>(args)...}};
  }

  /// @brief Copy-assign another Result to this instance.
  /// @param other the other instance
  /// @return *this, containing the contents of other
  Result& operator=(const Result& other) {
    if (this != &other) {
      data_ = other.data_;
    }
    return *this;
  }

  /// @brief Move-assign another Result to this instance.
  /// @param other the other instance
  /// @return *this, containing the contents of other
  Result& operator=(Result&& other) noexcept(
      std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_assignable_v<T>&&
          std::is_nothrow_move_constructible_v<E>&& std::is_nothrow_move_assignable_v<E>) {
    if (this != &other) {
      data_ = std::move(other.data_);
    }
    return *this;
  }

  /// @brief Put a new value into this instance, constructed in-place from the given arguments.
  /// @tparam Args the types of arguments given to this function
  /// @param args the arguments used for constructing the value
  template <typename... Args>
  void EmplaceValue(Args&&... args) {
    data_ = T{std::forward<Args>(args)...};
  }

  /// @brief Put a new error into this instance, constructed in-place from the given arguments.
  /// @tparam Args the types of arguments given to this function
  /// @param args the arguments used for constructing the error
  template <typename... Args>
  void EmplaceError(Args&&... args) {
    data_ = E{std::forward<Args>(args)...};
  }

  /// @brief Exchange the contents of this instance with those of other.
  /// @param other the other instance
  void Swap(Result& other) noexcept(
      std::is_nothrow_move_constructible_v<T>&& std::is_nothrow_move_assignable_v<T>&&
          std::is_nothrow_move_constructible_v<E>&& std::is_nothrow_move_assignable_v<E>) {
    data_.swap(other.data_);
  }

  /// @brief Check whether *this contains a value.
  /// @return true if *this contains a value, false otherwise
  bool HasValue() const noexcept { return data_.index() == kValueIdx; }

  /// @brief Check whether *this contains a value.
  explicit operator bool() const noexcept { return HasValue(); }

  /// @brief Access the contained value.
  /// This function's behavior is undefined if* this does not contain a value.
  /// @return a const_reference to the contained value
  const T& operator*() const& { return std::get<kValueIdx>(data_); }

  /// @brief Access the contained value.
  /// This function's behavior is undefined if* this does not contain a value.
  /// @return an rvalue reference to the contained value
  T&& operator*() && { return std::move(std::get<kValueIdx>(data_)); }

  /// @brief Access the contained value.
  /// This function's behavior is undefined if* this does not contain a value.
  /// @return a pointer to the contained value
  const T* operator->() const { return &std::get<kValueIdx>(data_); }

  /// @brief Access the contained value.
  /// The behavior of this function is undefined if* this does not contain a value.
  /// @return a const reference to the contained value
  const T& Value() const& { return std::get<kValueIdx>(data_); }

  /// @brief Access the contained value.
  /// The behavior of this function is undefined if* this does not contain a value.
  /// @return an rvalue reference to the contained value
  T&& Value() && { return std::move(std::get<kValueIdx>(data_)); }

  /// @brief Access the contained error.
  /// The behavior of this function is undefined if* this does not contain an error.
  /// @return a const reference to the contained error
  const E& Error() const& { return std::get<kErrorIdx>(data_); }

  /// @brief Access the contained error.
  /// The behavior of this function is undefined if* this does not contain an error.
  /// @return an rvalue reference to the contained error
  E&& Error() && { return std::move(std::get<kErrorIdx>(data_)); }

  /// @brief Return the contained value as an Optional.
  /// @return an Optional with the value, if present
  Optional<T> Ok() const& {
    if (HasValue()) {
      return Value();
    }

    return {};
  }

  /// @brief Return the contained value as an Optional.
  /// @return an Optional with the value, if present
  Optional<T> Ok() && {
    if (HasValue()) {
      return Value();
    }

    return {};
  }

  /// @brief Return the contained error as an Optional.
  /// @return an Optional with the error, if present
  Optional<E> Err() const& {
    if (!HasValue()) {
      return Error();
    }

    return {};
  }

  /// @brief Return the contained error as an Optional.
  /// @return an Optional with the error, if present
  Optional<E> Err() && {
    if (!HasValue()) {
      return Error();
    }

    return {};
  }

  /// @brief Return the contained value or the given default value.
  /// If *this contains a value, it is returned. Otherwise, the specified default value is returned, static_cast'd to T.
  /// @tparam U the type of defaultValue
  /// @param default_value the value to use if *this does not contain a value
  /// @return the value
  template <typename U>
  T ValueOr(U&& default_value) const& {
    if (HasValue()) {
      return Value();
    }

    return static_cast<T>(default_value);
  }

  /// @brief Return the contained value or the given default value.
  /// If *this contains a value, it is returned. Otherwise, the specified default value is returned, static_ cast'd to
  /// T.
  /// @tparam U the type of defaultValue
  /// @param default_value the value to use if *this does not contain a value
  /// @return the value
  template <typename U>
  T ValueOr(U&& default_value) && {
    if (HasValue()) {
      return Value();
    }

    return static_cast<T>(default_value);
  }

  /// @brief Return the contained error or the given default error.
  /// If *this contains an error, it is returned.Otherwise, the specified default error is returned,
  /// static_ cast'd to E.
  /// @tparam G the type of defaultError
  /// @param default_error the error to use if *this does not contain an error
  /// @return the error
  template <typename G>
  E ErrorOr(G&& default_error) const& {
    if (!HasValue()) {
      return Error();
    }

    return static_cast<E>(default_error);
  }

  /// @brief Return the contained error or the given default error.
  /// If *this contains an error, it is std::move'd into the return value. Otherwise, the specified default error is
  /// returned, static_cast'd to E.
  /// @tparam G the type of defaultError
  /// @param default_error the error to use if *this does not contain an error
  /// @return the error
  template <typename G>
  E ErrorOr(G&& default_error) && {
    if (!HasValue()) {
      return Error();
    }

    return static_cast<E>(default_error);
  }

  /// @brief Return whether this instance contains the given error.
  /// This call compares the argument error, static_cast'd to E, with the return value from Error().
  /// @tparam G the type of the error argument error
  /// @param error the error to check
  /// @return true if *this contains an error that is equivalent to the given error, false otherwise
  template <typename G>
  bool CheckError(G&& error) const& {
    if (HasValue()) {
      return false;
    }

    return static_cast<E>(error) == std::get<kErrorIdx>(data_);
  }

  /// @brief Return whether this instance contains the given error.
  /// This call compares the argument error, static_cast'd to E, with the return value from Error().
  /// @tparam G the type of the error argument error
  /// @param error the error to check
  /// @return true if *this contains an error that is equivalent to the given error, false otherwise
  template <typename G>
  bool CheckError(G&& error) && {
    if (HasValue()) {
      return false;
    }

    return static_cast<E>(error) == std::get<kErrorIdx>(data_);
  }

  /// @brief Return the contained value or throw an exception.
  /// This function does not participate in overload resolution when the compiler toolchain does not support C++
  /// exceptions.
  /// @exception TYPE the exception type associated with the contained error
  /// @return a const reference to the contained value
  const T& ValueOrThrow() const& noexcept(false) {
    if (HasValue()) {
      return Value();
    }
    throw NoValueException{Error()};
  }

  /// @brief Return the contained value or throw an exception.
  /// This function does not participate in overload resolution when the compiler toolchain does not support C++
  /// exceptions.
  /// @exception TYPE the exception type associated with the contained error
  /// @return a const reference to the contained value
  const T& ValueOrThrow() && noexcept(false) {
    if (HasValue()) {
      return Value();
    }
    throw NoValueException{Error()};
  }

  /// @brief Return the contained value or return the result of a function call.
  /// If *this contains a value, it is returned. Otherwise, the specified callable is invoked and its return
  /// value which is to be compatible to type T is returned from this function.
  /// The Callable is expected to be compatible to this interface: T f(const E&);
  /// @tparam F the type of the Callable f
  /// @param f the Callable
  /// @return the value
  template <typename F>
  T Resolve(F&& f) const {
    static_assert(std::is_same_v<F, std::function<T(const E&)>>,
                  "The Callable is expected to be compatible to this interface: T f(const E&);");
    if (HasValue()) {
      return Value();
    }

    return std::invoke(f, Error());
  }

  /// @brief Apply the given Callable to the value of this instance, and return a new Result with the result of the
  /// call.
  /// The Callable is expected to be compatible to one of these two interfaces:
  /// Result<XXX, E> f(const T&);
  /// XXX f(const T&);
  /// meaning that the Callable either returns a Result<XXX> or a XXX directly, where XXX can be any type that is
  /// suitable for use by class Result.
  /// The return type of this function is decltype(f(Value())) for a template argument F that returns a Result type, and
  /// it is Result<decltype(f(Value())), E> for a template argument F that does not return a Result type.
  /// If this instance does not contain a value, a new Result<XXX, E> is still created and returned, with the original
  /// error contents of this instance being copied into the new instance.
  /// @tparam F the type of the Callable f
  /// @param f the Callable
  /// @return a new Result instance of the possibly transformed type
  template <typename F>
  auto Bind(F&& f) const -> decltype(f(Value())) {
    using ReturnType = std::invoke_result_t<F, const T&>;
    return HasValue() ? std::invoke(f, Value()) : ReturnType{Error()};
  }

 private:
  static constexpr size_t kValueIdx{0};
  static constexpr size_t kErrorIdx{1};
  std::variant<T, E> data_;
};

/// @brief Specialization of class Result for "void" values.
/// @tparam E the type of error
template <typename E>
class Result<void, E> final {
 public:
  /// @brief Type alias for the type T of values, always "void" for this specialization .
  using value_type = void;
  /// @brief Type alias for the type E of errors.
  using error_type = E;

  /// @brief Construct a new Result with a "void" value.
  Result() noexcept = default;

  /// @brief Construct a new Result from the specified error (given as lvalue).
  /// @param e the error to put into the Result
  explicit Result(const E& e) : error_{e} {}

  /// @brief Construct a new Result from the specified error (given as rvalue).
  /// @param e the error to put into the Result
  explicit Result(E&& e) : error_{std::move(e)} {}

  /// @brief Copy-construct a new Result from another instance.
  /// @param other the other instance
  Result(const Result& other) = default;

  /// @brief Move-construct a new Result from another instance.
  /// @param other the other instance
  Result(Result&& other) = default;

  /// @brief Destructor.
  /// This destructor is trivial if std::is_trivially_destructible<E>::value is true.
  ~Result() noexcept = default;

  /// @brief Build a new Result with "void" as value.
  /// @return a Result that contains a "void" value
  static Result FromValue() noexcept { return {}; }

  /// @brief Build a new Result from the specified error (given as lvalue).
  /// @param e the error to put into the Result
  /// @return a Result that contains the error e
  static Result FromError(const E& e) { return Result{e}; }

  /// @brief Build a new Result from the specified error (given as rvalue).
  /// @param e the error to put into the Result
  /// @return a Result that contains the error e
  static Result FromError(E&& e) { return {std::move(e)}; }

  /// @brief Build a new Result from an error that is constructed in-place from the given arguments.
  /// This function shall not participate in overload resolution unless : std::is_constructible<E, Args&&...>::value is
  /// true, and the first type of the expanded parameter pack is not E, and the first type of the expanded parameter
  /// pack is not a specialization of Result
  /// @tparam Args the types of arguments given to this function
  /// @param args the parameter pack used for constructing the error
  /// @return a Result that contains an error
  template <typename... Args, std::enable_if_t<std::is_constructible_v<E, Args&&...>, bool> = true>
  static Result FromError(Args&&... args) {
    return Result{E{std::forward<Args&&>(args)...}};
  }

  /// @brief Copy-assign another Result to this instance.
  /// @param other the other instance
  /// @return *this, containing the contents of other
  Result& operator=(const Result& other) {
    if (this != &other) {
      error_ = other.error_;
    }
    return *this;
  }

  /// @brief Move-assign another Result to this instance.
  /// @param other the other instance
  /// @return *this, containing the contents of other
  Result& operator=(Result&& other) noexcept(
      std::is_nothrow_move_constructible_v<E>&& std::is_nothrow_move_assignable_v<E>) {
    if (this != &other) {
      error_ = std::move(other.error_);
    }
    return *this;
  }

  /// @brief Put a new value into this instance, constructed in-place from the given arguments.
  /// @tparam Args the types of arguments given to this function
  template <typename... Args>
  void EmplaceValue(Args&&...) noexcept {}

  /// @brief Put a new error into this instance, constructed in-place from the given arguments.
  /// @tparam Args the types of arguments given to this function
  /// @param args the arguments used for constructing the error
  template <typename... Args>
  void EmplaceError(Args&&... args) noexcept {
    error_ = {std::forward<Args>(args)...};
  }

  /// @brief Exchange the contents of this instance with those of other.
  /// @param other the other instance
  void Swap(Result& other) {
    if (this != &other) {
      error_.swap(other.error_);
    }
  }

  /// @brief Check whether *this contains a value.
  /// @return true if *this contains a value, false otherwise
  bool HasValue() const noexcept { return !error_; }

  /// @brief Check whether *this contains a value.
  explicit operator bool() const noexcept { return HasValue(); }

  /// @brief Do nothing.
  /// This function only exists for helping with generic programming. The behavior of this function is undefined if
  /// *this does not contain a value.
  void operator*() const {}

  /// @brief Do nothing.
  /// This function only exists for helping with generic programming. The behavior of this function is undefined if
  /// *this does not contain a value.
  void Value() const {}

  /// @brief Access the contained error.
  /// The behavior of this function is undefined if* this does not contain an error.
  /// @return a const reference to the contained error
  const E& Error() const& { return *error_; }

  /// @brief Access the contained error.
  /// The behavior of this function is undefined if* this does not contain an error.
  /// @return a rvalue reference to the contained error
  E&& Error() && { return std::move(*error_); }

  /// @brief Return the contained error as an Optional.
  /// @return an Optional with the error, if present
  Optional<E> Err() const& { return error_; }

  /// @brief Return the contained error as an Optional.
  /// @return an Optional with the error, if present
  Optional<E> Err() && { return std::move(error_); }

  /// @brief Do nothing.
  /// This function only exists for helping with generic programming.
  /// @tparam U the type of defaultValue
  template <typename U>
  void ValueOr(U&&) const {}

  /// @brief Return the contained error or the given default error.
  /// If *this contains an error, it is returned. Otherwise, the specified default error is returned, static_ cast'd to
  /// E.
  /// @tparam G the type of defaultError
  /// @param default_error the error to use if *this does not contain an error
  /// @return the error
  template <typename G>
  E ErrorOr(G&& default_error) const& {
    if (HasValue()) {
      return static_cast<E>(default_error);
    }
    return *error_;
  }

  /// @brief Return the contained error or the given default error.
  /// If *this contains an error, it is std::move'd into the return value. Otherwise, the specified default error is
  /// returned, static_ cast'd to E.
  /// @tparam G the type of defaultError
  /// @param default_error the error to use if *this does not contain an error
  /// @return the error
  template <typename G>
  E ErrorOr(G&& default_error) && {
    if (HasValue()) {
      return std::forward<E>(default_error);
    }
    return *error_;
  }

  /// @brief Return whether this instance contains the given error.
  /// This call compares the argument error, static_cast'd to E, with the return value from Error().
  /// @tparam G the type of the error argument error
  /// @param error the error to check
  /// @return true if *this contains an error that is equivalent to the given error, false otherwise
  template <typename G>
  bool CheckError(G&& error) const {
    return *error_ == static_cast<E>(error);
  }

  /// @brief Return the contained value or throw an exception.
  /// This function does not participate in overload resolution when the compiler toolchain does not support C++
  /// exceptions.
  /// @exception <TYPE> the exception type associated with the contained error
  void ValueOrThrow() const noexcept(false) {
    if (!HasValue()) {
      throw NoValueException(Error());
    }
  }

  /// @brief Do nothing or call a function.
  /// If *this contains a value, this function does nothing.Otherwise,
  /// the specified callable is invoked.The Callable is expected to be compatible to this interface : void f(const E&);
  /// This function only exists for helping with generic programming.
  /// @tparam F the type of the Callable f
  /// @param f the Callable
  template <typename F>
  void Resolve(F&& f) const {
    if (!HasValue()) {
      std::invoke(f, Error());
    }
  }

  /// @brief Call the given Callable, and return a new Result with the result of the call.
  /// The Callable is expected to be compatible to one of these two interfaces :
  /// Result<XXX, E> f();
  /// XXX f();
  /// meaning that the Callable either returns a Result<XXX, E> or a XXX directly, where XXX can be any type that is
  /// suitable for use by class Result. The return type of this function is decltype(f()) for a template argument F that
  /// returns a Result type, and it is Result<decltype(f()), E> for a template argument F that does not return a Result
  /// type.
  /// If this instance does not contain a value, a new Result<XXX, E> is still created and returned, with the original
  /// error contents of this instance being copied into the new instance.
  /// @tparam F the type of the Callable f
  /// @param f the Callable
  /// @return a new Result instance of the possibly transformed type
  template <typename F>
  auto Bind(F&& f) const -> decltype(f()) {
    using ReturnType = std::invoke_result_t<F>;
    return HasValue() ? std::invoke(f) : ReturnType{Error()};
  }

 private:
  Optional<E> error_;
};
}  // namespace ara::core

#endif