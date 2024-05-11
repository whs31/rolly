#pragma once

#include <string>
#include <initializer_list>
#include <bits/exception.h>
#include <bits/invoke.h>
#include <bits/stl_construct.h>
#include <bits/utility.h>
#include <leaf/global/sourcelocation.h>

namespace leaf
{
  [[noreturn]] inline auto panic(std::string_view message, source_location location = source_location::current()) -> void;

  namespace rs
  {
    using std::string;
    using std::exception;

    template <typename T = void, typename E = std::string>
    class Result;

    template <typename E = string>
    class Error;

    template <typename E = string>
    class bad_result_access;

    template <>
    class bad_result_access<void> : public exception
    {
     protected:
      bad_result_access() noexcept = default;
      bad_result_access(bad_result_access const&) = default;
      bad_result_access(bad_result_access&&) = default;
      ~bad_result_access() override = default;
      auto operator=(bad_result_access const&) -> bad_result_access& = default;
      auto operator=(bad_result_access&&) -> bad_result_access& = default;

     public:
      [[nodiscard]] auto what() const noexcept -> const char* override {
        return "bad access to Result<T, E> object value";
      }
    };

    template <typename E>
    class bad_result_access : public bad_result_access<void>
    {
     public:
      explicit bad_result_access(E e)
        : m_val(std::move(e))
      {}

      [[nodiscard]] auto error() & noexcept -> E& { return this->m_val; }
      [[nodiscard]] auto error() const & noexcept -> E const& { return this->m_val; }
      [[nodiscard]] auto error() && noexcept -> E&& { return std::move(this->m_val); }
      [[nodiscard]] auto error() const && noexcept -> E const&& { return std::move(this->m_val); }

     private:
      E m_val;
    };

    struct unexpect_t
    {
      explicit constexpr unexpect_t() = default;
    };

    inline constexpr unexpect_t unexpect {};

    namespace detail
    {
      template <typename T>
      constexpr bool _is_expected = false;

      template <typename T, typename E>
      constexpr bool _is_expected<Result<T, E>> = true;

      template <typename E>
      constexpr bool _is_unexpected = false;

      template <typename E>
      constexpr bool _is_unexpected<Error<E>> = true;

      template <typename F, typename T>
      using _result = std::remove_cvref_t<std::invoke_result_t<F&&, T&&>>;

      template <typename F, typename T>
      using _result_xform = std::remove_cv_t<std::invoke_result_t<F&&, T&&>>;

      template <typename F>
      using _result0 = std::remove_cvref_t<std::invoke_result_t<F&&>>;

      template <typename F>
      using _result0_xform = std::remove_cv_t<std::invoke_result_t<F&&>>;

      template <typename E>
      concept CanBeUnexpected = std::is_object_v<E>
        and (not std::is_array_v<E>)
        and (not detail::_is_unexpected<E>)
        and (not std::is_const_v<E>)
        and (not std::is_volatile_v<E>);

      struct _in_place_inv {};
      struct _unexpect_inv {};
    }

    template <typename E>
    class Error
    {
      static_assert(detail::CanBeUnexpected<E>);

     public:
      constexpr Error(Error const&) = default;
      constexpr Error(Error&&) = default;

      template <typename Err = E>
      requires (not std::is_same_v<std::remove_cvref_t<Err>, Error>)
        and (not std::is_same_v<std::remove_cvref_t<Err>, std::in_place_t>)
        and std::is_constructible_v<E, Err>
      constexpr explicit Error(Err&& e) noexcept(std::is_nothrow_constructible_v<E, Err>)
        : m_val(std::forward<Err>(e))
      {}

      template <typename... Args>
      requires std::is_constructible_v<E, Args...>
      constexpr explicit Error(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
        : m_val(std::forward<Args>(args)...)
      {}

      template <typename U, typename... Args>
      requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
      constexpr explicit Error(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        : m_val(il, std::forward<Args>(args)...)
      {}

      constexpr auto operator=(Error const&) -> Error& = default;
      constexpr auto operator=(Error&&) -> Error& = default;

      [[nodiscard]] constexpr auto error() const & noexcept -> const E& { return this->m_val; }
      [[nodiscard]] constexpr auto error() & noexcept -> E& { return this->m_val; }
      [[nodiscard]] constexpr auto error() const && noexcept -> const E&& { return std::move(this->m_val); }
      [[nodiscard]] constexpr auto error() && noexcept -> E&& { return std::move(this->m_val); }

      constexpr auto swap(Error& rhs) noexcept(std::is_nothrow_swappable_v<E>) -> void {
        static_assert(std::is_swappable_v<E>);
        std::swap(this->m_val, rhs.m_val);
      }

      template <typename Err>
      [[nodiscard]] friend constexpr auto operator==(Error const& lhs, Err const& rhs) -> bool {
        return lhs.m_val == rhs.error();
      }

      friend constexpr auto swap(Error& lhs, Error& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void
      requires std::is_swappable_v<E> {
        lhs.swap(rhs);
      }

     private:
      E m_val;
    };

    template <typename E>
    Error(E) -> Error<E>;

    namespace detail
    {
      template <typename T>
      struct guard
      {
        static_assert(std::is_nothrow_move_constructible_v<T>);

        constexpr explicit guard(T& x)
          : m_guarded(__builtin_addressof(x))
          , m_tmp(std::move(x))
        {
          std::destroy_at(this->m_guarded);
        }

        constexpr ~guard() {
          if(this->m_guarded) [[unlikely]]
            std::construct_at(this->m_guarded, std::move(this->m_tmp));
        }

        guard(guard const&) = delete;
        auto operator=(guard const&) -> guard& = delete;

        constexpr auto release() noexcept -> T& {
          this->m_guarded = nullptr;
          return std::move(this->m_tmp);
        }

       private:
        T* m_guarded;
        T m_tmp;
      };

      template <typename T, typename U, typename V>
      constexpr auto _reinit(T* new_val, U* old_val, V&& arg) noexcept(std::is_nothrow_constructible_v<T, V>) -> void
      {
        if constexpr(std::is_nothrow_constructible_v<T, V>) {
          std::destroy_at(old_val);
          std::construct_at(new_val, std::forward<V>(arg));
        }
        else if constexpr(std::is_nothrow_move_constructible_v<T>) {
          auto tmp = T(std::forward<V>(arg));
          std::destroy_at(old_val);
          std::construct_at(new_val, std::move(tmp));
        }
        else {
          auto g = guard<U>(*old_val);
          std::construct_at(new_val, std::forward<V>(arg));
          g.release();
        }
      }
    } // namespace detail

    template <typename T, typename E>
    class Result
    {
      static_assert(not std::is_reference_v<T>);
      static_assert(not std::is_function_v<T>);
      static_assert(not std::is_same_v<std::remove_cv_t<T>, std::in_place_t>);
      static_assert(not std::is_same_v<std::remove_cv_t<T>, unexpect_t>);
      static_assert(not detail::_is_unexpected<std::remove_cv_t<T>>);
      static_assert(detail::CanBeUnexpected<E>);

      template <typename U, typename Err, typename Un = Error<E>>
      static constexpr auto cons_from_expected = bool(
        std::__or_v<std::is_constructible<T, Result<U, Err>&>,
        std::is_constructible<T, Result<U, Err> const&>,
        std::is_constructible<T, Result<U, Err>>,
        std::is_constructible<Result<U, Err>&, T>,
        std::is_constructible<Result<U, Err>, T>,
        std::is_constructible<Result<U, Err> const&, T>,
        std::is_constructible<Result<U, Err> const, T>,
        std::is_constructible<Un, Result<U, Err>&>,
        std::is_constructible<Un, Result<U, Err>>,
        std::is_constructible<Un, Result<U, Err> const&>,
        std::is_constructible<Un, Result<U, Err> const>
      >);

      template <typename U, typename Err>
      constexpr static auto explicit_conv = bool(
        std::__or_v<std::__not_<std::is_convertible<U, T>>,
        std::__not_<std::is_convertible<Err, E>>>
      );

      template <typename U>
      static constexpr auto same_val = bool(std::is_same_v<typename U::value_type, T>);

      template <typename U>
      static constexpr auto same_err = bool(std::is_same_v<typename U::error_type, E>);

     public:
      using value_type = T;
      using error_type = E;
      using unexpected_type = Error<E>;

      template <typename U>
      using rebind = Result<U, E>;

      constexpr Result() noexcept(std::is_nothrow_default_constructible_v<T>)
      requires std::is_default_constructible_v<T>
        : m_val()
        , m_has_value(true)
      {}

      Result(Result const&) = default;
      constexpr Result(Result const& x) noexcept(std::__and_v<std::is_nothrow_copy_constructible<T>, std::is_nothrow_copy_constructible<E>>)
        requires std::is_copy_constructible_v<T>
        and std::is_copy_constructible_v<E>
        and (not std::is_trivially_copy_constructible_v<T> or not std::is_trivially_copy_constructible_v<E>)
        : m_has_value(x.m_has_value)
      {
        if(this->m_has_value)
          std::construct_at(__builtin_addressof(this->m_val), x.m_val);
        else
          std::construct_at(__builtin_addressof(this->m_unex), x.m_unex);
      }

      Result(Result&&) = default;
      constexpr Result(Result&& x) noexcept(std::__and_v<std::is_nothrow_move_constructible<T>, std::is_nothrow_move_constructible<E>>)
        requires std::is_move_constructible_v<T>
        and std::is_move_constructible_v<E>
        and (not std::is_trivially_move_constructible_v<T> or not std::is_trivially_move_constructible_v<E>)
        : m_has_value(x.m_has_value)
      {
        if(this->m_has_value)
          std::construct_at(__builtin_addressof(this->m_val), std::move(x.m_val));
        else
          std::construct_at(__builtin_addressof(this->m_unex), std::move(x.m_unex));
      }

      template <typename U, typename G>
      requires std::is_constructible_v<T, U const&>
        and std::is_constructible_v<E, G const&>
        and (not cons_from_expected<U, G>)
      constexpr explicit(explicit_conv<U const&, G const&>) Result(Result<U, G> const& x)
        noexcept(std::__and_v<std::is_nothrow_constructible<T, U const&>, std::is_nothrow_constructible<E, G const&>>)
        : m_has_value(x.m_has_value)
      {
        if(this->m_has_value)
          std::construct_at(__builtin_addressof(this->m_val), x.m_val);
        else
          std::construct_at(__builtin_addressof(this->m_unex), x.m_unex);
      }

      template <typename U = T>
      requires (not std::is_same_v<std::remove_cvref_t<U>, Result>)
        and (not std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>)
        and (not detail::_is_unexpected<std::remove_cvref_t<U>>)
        and std::is_constructible_v<T, U>
      constexpr explicit(not std::is_convertible_v<U, T>) Result(U&& v)
        noexcept(std::is_nothrow_constructible_v<T, U>)
        : m_val(std::forward<U>(v))
        , m_has_value(true)
      {}

      template <typename G = E>
      requires std::is_constructible_v<E, G const&>
      constexpr explicit(not std::is_convertible_v<G const&, E>) Result(Error<G> const& u)
        noexcept(std::is_nothrow_constructible_v<E, G const&>)
        : m_unex(u.error())
        , m_has_value(false)
      {}

      template <typename G = E>
      requires std::is_constructible_v<E, G>
      constexpr explicit(not std::is_convertible_v<G, E>) Result(Error<G>&& u)
        noexcept(std::is_nothrow_constructible_v<E, G>)
        : m_unex(std::move(u).error())
        , m_has_value(false)
      {}

      template <typename... Args>
      requires std::is_constructible_v<T, Args...>
      constexpr explicit Result(std::in_place_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
        : m_val(std::forward<Args>(args)...)
        , m_has_value(true)
      {}

      template <typename U, typename... Args>
      requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
      constexpr explicit Result(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
        : m_val(il, std::forward<Args>(args)...)
        , m_has_value(true)
      {}

      template <typename... Args>
      requires std::is_constructible_v<E, Args...>
      constexpr explicit Result(unexpect_t, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, Args...>)
        : m_unex(std::forward<Args>(args)...)
        , m_has_value(false)
      {}

      template <typename U, typename... Args>
      requires std::is_constructible_v<E, std::initializer_list<U>&, Args...>
      constexpr explicit Result(unexpect_t, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        : m_unex(il, std::forward<Args>(args)...)
        , m_has_value(false)
      {}

      constexpr ~Result() = default;
      constexpr ~Result() requires (not std::is_trivially_destructible_v<T>) or (not std::is_trivially_destructible_v<E>) {
        if(this->m_has_value)
          std::destroy_at(__builtin_addressof(this->m_val));
        else
          std::destroy_at(__builtin_addressof(this->m_unex));
      }

      auto operator=(const Result&) -> Result& = delete;

      constexpr Result& operator=(Result const& x)
        noexcept(std::__and_v<std::is_nothrow_copy_constructible<T>,
          std::is_nothrow_copy_constructible<E>,
          std::is_nothrow_copy_assignable<T>,
          std::is_nothrow_copy_assignable<E>>)
        requires std::is_copy_assignable_v<T>
          and std::is_copy_constructible_v<T>
          and std::is_copy_assignable_v<E>
          and std::is_copy_constructible_v<E>
          and (std::is_nothrow_move_constructible_v<T> or std::is_nothrow_move_constructible_v<E>) {
        if(x.m_has_value)
          this->m_assign_val(x.m_val);
        else
          this->m_assign_unex(x.m_unex);
        return *this;
      }

      constexpr Result& operator=(Result&& x)
        noexcept(std::__and_v<std::is_nothrow_copy_constructible<T>,
          std::is_nothrow_copy_constructible<E>,
          std::is_nothrow_copy_assignable<T>,
          std::is_nothrow_copy_assignable<E>>)
        requires std::is_move_assignable_v<T>
          and std::is_move_constructible_v<T>
          and std::is_move_assignable_v<E>
          and std::is_move_constructible_v<E>
          and (std::is_nothrow_move_constructible_v<T> or std::is_nothrow_move_constructible_v<E>) {
        if(x.m_has_value)
          this->m_assign_val(std::move(x.m_val));
        else
          this->m_assign_unex(std::move(x.m_unex));
        return *this;
      }

      template <typename U = T>
        requires (not std::is_same_v<Result, std::remove_cvref_t<U>>)
          and (not detail::_is_unexpected<std::remove_cvref_t<U>>)
          and std::is_constructible_v<T, U>
          and std::is_assignable_v<T&, U>
          and (std::is_nothrow_constructible_v<T, U>
            or std::is_nothrow_move_constructible_v<T>
            or std::is_nothrow_move_constructible_v<E>
          )
      constexpr Result& operator=(U&& v) {
        this->m_assign_val(std::forward<U>(v));
        return *this;
      }

      template <typename G>
      requires std::is_constructible_v<E, G const&>
        and std::is_assignable_v<E&, G const&>
        and (std::is_nothrow_constructible_v<E, G const&>
          or std::is_nothrow_move_constructible_v<T>
          or std::is_nothrow_move_constructible_v<E>
        )
      constexpr Result& operator=(Error<G> const& e) {
        this->m_assign_unex(e.error());
        return *this;
      }

      template <typename G>
      requires std::is_constructible_v<E, G>
        and std::is_assignable_v<E&, G>
        and (std::is_nothrow_constructible_v<E, G>
          or std::is_nothrow_move_constructible_v<T>
          or std::is_nothrow_move_constructible_v<E>
        )
      constexpr Result& operator=(Error<G>&& e) {
        this->m_assign_unex(std::move(e).error());
        return *this;
      }

      template <typename... Args>
      requires std::is_nothrow_constructible_v<T, Args...>
      constexpr T& emplace(Args&&... args) noexcept {
        if(this->m_has_value)
          std::destroy_at(__builtin_addressof(this->m_val));
        else {
          std::destroy_at(__builtin_addressof(this->m_unex));
          this->m_has_value = true;
        }
        std::construct_at(__builtin_addressof(this->m_val), std::forward<Args>(args)...);
        return this->m_val;
      }

      template <typename U, typename... Args>
      requires std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>
      constexpr T& emplace(std::initializer_list<U> il, Args&&... args) noexcept {
        if(this->m_has_value)
          std::destroy_at(__builtin_addressof(this->m_val));
        else {
          std::destroy_at(__builtin_addressof(this->m_unex));
          this->m_has_value = true;
        }
        std::construct_at(__builtin_addressof(this->m_val), il, std::forward<Args>(args)...);
        return this->m_val;
      }

      constexpr void swap(Result& x)
        noexcept(std::__and_v<std::is_nothrow_move_constructible<T>,
                         std::is_nothrow_move_constructible<E>,
                         std::is_nothrow_swappable<T&>,
                         std::is_nothrow_swappable<E&>>)
        requires std::is_swappable_v<T>
          and std::is_swappable_v<E>
          and std::is_move_constructible_v<T>
          and std::is_move_constructible_v<E>
          and (std::is_nothrow_move_constructible_v<T> or std::is_nothrow_move_constructible_v<E>) {
        if(this->m_has_value) {
          if(x.m_has_value)
            std::swap(this->m_val, x.m_val);
          else
            this->m_swap_val_unex(x);
        }
        else {
          if(x.m_has_value)
            x.m_swap_val_unex(*this);
          else
            std::swap(this->m_unex, x.m_unex);
        }
      }

      [[nodiscard]] constexpr const T* operator->() const noexcept
      {
        __glibcxx_assert(_M_has_value);
        return __builtin_addressof(_M_val);
      }

      [[nodiscard]]
      constexpr _Tp*
      operator->() noexcept
      {
        __glibcxx_assert(_M_has_value);
        return __builtin_addressof(_M_val);
      }

      [[nodiscard]]
      constexpr const _Tp&
      operator*() const & noexcept
      {
        __glibcxx_assert(_M_has_value);
        return _M_val;
      }

      [[nodiscard]]
      constexpr _Tp&
      operator*() & noexcept
      {
        __glibcxx_assert(_M_has_value);
        return _M_val;
      }

      [[nodiscard]]
      constexpr const _Tp&&
      operator*() const && noexcept
      {
        __glibcxx_assert(_M_has_value);
        return std::move(_M_val);
      }

      [[nodiscard]]
      constexpr _Tp&&
      operator*() && noexcept
      {
        __glibcxx_assert(_M_has_value);
        return std::move(_M_val);
      }

      [[nodiscard]]
      constexpr explicit
      operator bool() const noexcept { return _M_has_value; }

      [[nodiscard]]
      constexpr bool has_value() const noexcept { return _M_has_value; }

      constexpr const _Tp&
      value() const &
      {
        if (_M_has_value) [[likely]]
          return _M_val;
        _GLIBCXX_THROW_OR_ABORT(bad_expected_access<_Er>(_M_unex));
      }

      constexpr _Tp&
      value() &
      {
        if (_M_has_value) [[likely]]
          return _M_val;
        const auto& __unex = _M_unex;
        _GLIBCXX_THROW_OR_ABORT(bad_expected_access<_Er>(__unex));
      }

      constexpr const _Tp&&
      value() const &&
      {
        if (_M_has_value) [[likely]]
          return std::move(_M_val);
        _GLIBCXX_THROW_OR_ABORT(bad_expected_access<_Er>(std::move(_M_unex)));
      }

      constexpr _Tp&&
      value() &&
      {
        if (_M_has_value) [[likely]]
          return std::move(_M_val);
        _GLIBCXX_THROW_OR_ABORT(bad_expected_access<_Er>(std::move(_M_unex)));
      }

      constexpr const _Er&
      error() const & noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return _M_unex;
      }

      constexpr _Er&
      error() & noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return _M_unex;
      }

      constexpr const _Er&&
      error() const && noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return std::move(_M_unex);
      }

      constexpr _Er&&
      error() && noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return std::move(_M_unex);
      }

      template<typename _Up>
      constexpr _Tp
      value_or(_Up&& __v) const &
        noexcept(__and_v<is_nothrow_copy_constructible<_Tp>,
                         is_nothrow_convertible<_Up, _Tp>>)
      {
        static_assert( is_copy_constructible_v<_Tp> );
        static_assert( is_convertible_v<_Up, _Tp> );

        if (_M_has_value)
          return _M_val;
        return static_cast<_Tp>(std::forward<_Up>(__v));
      }

      template<typename _Up>
      constexpr _Tp
      value_or(_Up&& __v) &&
        noexcept(__and_v<is_nothrow_move_constructible<_Tp>,
                         is_nothrow_convertible<_Up, _Tp>>)
      {
        static_assert( is_move_constructible_v<_Tp> );
        static_assert( is_convertible_v<_Up, _Tp> );

        if (_M_has_value)
          return std::move(_M_val);
        return static_cast<_Tp>(std::forward<_Up>(__v));
      }

      template<typename _Gr = _Er>
      constexpr _Er
      error_or(_Gr&& __e) const&
      {
        static_assert( is_copy_constructible_v<_Er> );
        static_assert( is_convertible_v<_Gr, _Er> );

        if (_M_has_value)
          return std::forward<_Gr>(__e);
        return _M_unex;
      }

      template<typename _Gr = _Er>
      constexpr _Er
      error_or(_Gr&& __e) &&
      {
        static_assert( is_move_constructible_v<_Er> );
        static_assert( is_convertible_v<_Gr, _Er> );

        if (_M_has_value)
          return std::forward<_Gr>(__e);
        return std::move(_M_unex);
      }

      // monadic operations

      template<typename _Fn> requires is_constructible_v<_Er, _Er&>
      constexpr auto
      and_then(_Fn&& __f) &
      {
        using _Up = __expected::__result<_Fn, _Tp&>;
        static_assert(__expected::__is_expected<_Up>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Up::error_type, _Er>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected with the same error_type");

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f), _M_val);
        else
          return _Up(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er&>
      constexpr auto
      and_then(_Fn&& __f) const &
      {
        using _Up = __expected::__result<_Fn, const _Tp&>;
        static_assert(__expected::__is_expected<_Up>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Up::error_type, _Er>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected with the same error_type");

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f), _M_val);
        else
          return _Up(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, _Er>
      constexpr auto
      and_then(_Fn&& __f) &&
      {
        using _Up = __expected::__result<_Fn, _Tp&&>;
        static_assert(__expected::__is_expected<_Up>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Up::error_type, _Er>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected with the same error_type");

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f), std::move(_M_val));
        else
          return _Up(unexpect, std::move(_M_unex));
      }


      template<typename _Fn> requires is_constructible_v<_Er, const _Er>
      constexpr auto
      and_then(_Fn&& __f) const &&
      {
        using _Up = __expected::__result<_Fn, const _Tp&&>;
        static_assert(__expected::__is_expected<_Up>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Up::error_type, _Er>,
                      "the function passed to std::expected<T, E>::and_then "
                      "must return a std::expected with the same error_type");

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f), std::move(_M_val));
        else
          return _Up(unexpect, std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Tp, _Tp&>
      constexpr auto
      or_else(_Fn&& __f) &
      {
        using _Gr = __expected::__result<_Fn, _Er&>;
        static_assert(__expected::__is_expected<_Gr>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Gr::value_type, _Tp>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected with the same value_type");

        if (has_value())
          return _Gr(in_place, _M_val);
        else
          return std::__invoke(std::forward<_Fn>(__f), _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Tp, const _Tp&>
      constexpr auto
      or_else(_Fn&& __f) const &
      {
        using _Gr = __expected::__result<_Fn, const _Er&>;
        static_assert(__expected::__is_expected<_Gr>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Gr::value_type, _Tp>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected with the same value_type");

        if (has_value())
          return _Gr(in_place, _M_val);
        else
          return std::__invoke(std::forward<_Fn>(__f), _M_unex);
      }


      template<typename _Fn> requires is_constructible_v<_Tp, _Tp>
      constexpr auto
      or_else(_Fn&& __f) &&
      {
        using _Gr = __expected::__result<_Fn, _Er&&>;
        static_assert(__expected::__is_expected<_Gr>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Gr::value_type, _Tp>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected with the same value_type");

        if (has_value())
          return _Gr(in_place, std::move(_M_val));
        else
          return std::__invoke(std::forward<_Fn>(__f), std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Tp, const _Tp>
      constexpr auto
      or_else(_Fn&& __f) const &&
      {
        using _Gr = __expected::__result<_Fn, const _Er&&>;
        static_assert(__expected::__is_expected<_Gr>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected");
        static_assert(is_same_v<typename _Gr::value_type, _Tp>,
                      "the function passed to std::expected<T, E>::or_else "
                      "must return a std::expected with the same value_type");

        if (has_value())
          return _Gr(in_place, std::move(_M_val));
        else
          return std::__invoke(std::forward<_Fn>(__f), std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Er, _Er&>
      constexpr auto
      transform(_Fn&& __f) &
      {
        using _Up = __expected::__result_xform<_Fn, _Tp&>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 _M_val);
          });
        else
          return _Res(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er&>
      constexpr auto
      transform(_Fn&& __f) const &
      {
        using _Up = __expected::__result_xform<_Fn, const _Tp&>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 _M_val);
          });
        else
          return _Res(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, _Er>
      constexpr auto
      transform(_Fn&& __f) &&
      {
        using _Up = __expected::__result_xform<_Fn, _Tp>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 std::move(_M_val));
          });
        else
          return _Res(unexpect, std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er>
      constexpr auto
      transform(_Fn&& __f) const &&
      {
        using _Up = __expected::__result_xform<_Fn, const _Tp>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 std::move(_M_val));
          });
        else
          return _Res(unexpect, std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Tp, _Tp&>
      constexpr auto
      transform_error(_Fn&& __f) &
      {
        using _Gr = __expected::__result_xform<_Fn, _Er&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res(in_place, _M_val);
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 _M_unex);
          });
      }

      template<typename _Fn> requires is_constructible_v<_Tp, const _Tp&>
      constexpr auto
      transform_error(_Fn&& __f) const &
      {
        using _Gr = __expected::__result_xform<_Fn, const _Er&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res(in_place, _M_val);
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 _M_unex);
          });
      }

      template<typename _Fn> requires is_constructible_v<_Tp, _Tp>
      constexpr auto
      transform_error(_Fn&& __f) &&
      {
        using _Gr = __expected::__result_xform<_Fn, _Er&&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res(in_place, std::move(_M_val));
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 std::move(_M_unex));
          });
      }

      template<typename _Fn> requires is_constructible_v<_Tp, const _Tp>
      constexpr auto
      transform_error(_Fn&& __f) const &&
      {
        using _Gr = __expected::__result_xform<_Fn, const _Er&&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res(in_place, std::move(_M_val));
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 std::move(_M_unex));
          });
      }

      // equality operators

      template<typename _Up, typename _Er2>
        requires (!is_void_v<_Up>)
      friend constexpr bool
      operator==(const expected& __x, const expected<_Up, _Er2>& __y)
      // FIXME: noexcept(noexcept(bool(*__x == *__y))
      // && noexcept(bool(__x.error() == __y.error())))
      {
        if (__x.has_value())
          return __y.has_value() && bool(*__x == *__y);
        else
          return !__y.has_value() && bool(__x.error() == __y.error());
      }

      template<typename _Up>
      friend constexpr bool
      operator==(const expected& __x, const _Up& __v)
      // FIXME: noexcept(noexcept(bool(*__x == __v)))
      { return __x.has_value() && bool(*__x == __v); }

      template<typename _Er2>
      friend constexpr bool
      operator==(const expected& __x, const unexpected<_Er2>& __e)
      // FIXME: noexcept(noexcept(bool(__x.error() == __e.error())))
      { return !__x.has_value() && bool(__x.error() == __e.error()); }

      friend constexpr void
      swap(expected& __x, expected& __y)
        noexcept(noexcept(__x.swap(__y)))
        requires requires {__x.swap(__y);}
      { __x.swap(__y); }

     private:
      template<typename, typename> friend class expected;

      template<typename _Vp>
      constexpr void
      _M_assign_val(_Vp&& __v)
      {
        if (_M_has_value)
          _M_val = std::forward<_Vp>(__v);
        else
        {
          __expected::__reinit(__builtin_addressof(_M_val),
                               __builtin_addressof(_M_unex),
                               std::forward<_Vp>(__v));
          _M_has_value = true;
        }
      }

      template<typename _Vp>
      constexpr void
      _M_assign_unex(_Vp&& __v)
      {
        if (_M_has_value)
        {
          __expected::__reinit(__builtin_addressof(_M_unex),
                               __builtin_addressof(_M_val),
                               std::forward<_Vp>(__v));
          _M_has_value = false;
        }
        else
          _M_unex = std::forward<_Vp>(__v);
      }

      // Swap two expected objects when only one has a value.
      // Precondition: this->_M_has_value && !__rhs._M_has_value
      constexpr void
      _M_swap_val_unex(expected& __rhs)
        noexcept(__and_v<is_nothrow_move_constructible<_Er>,
                         is_nothrow_move_constructible<_Tp>>)
      {
        if constexpr (is_nothrow_move_constructible_v<_Er>)
        {
          __expected::_Guard<_Er> __guard(__rhs._M_unex);
          std::construct_at(__builtin_addressof(__rhs._M_val),
                            std::move(_M_val)); // might throw
          __rhs._M_has_value = true;
          std::destroy_at(__builtin_addressof(_M_val));
          std::construct_at(__builtin_addressof(_M_unex),
                            __guard.release());
          _M_has_value = false;
        }
        else
        {
          __expected::_Guard<_Tp> __guard(_M_val);
          std::construct_at(__builtin_addressof(_M_unex),
                            std::move(__rhs._M_unex)); // might throw
          _M_has_value = false;
          std::destroy_at(__builtin_addressof(__rhs._M_unex));
          std::construct_at(__builtin_addressof(__rhs._M_val),
                            __guard.release());
          __rhs._M_has_value = true;
        }
      }

      using __in_place_inv = __expected::__in_place_inv;
      using __unexpect_inv = __expected::__unexpect_inv;

      template<typename _Fn>
      explicit constexpr
        expected(__in_place_inv, _Fn&& __fn)
        : _M_val(std::forward<_Fn>(__fn)()), _M_has_value(true)
      { }

      template<typename _Fn>
      explicit constexpr
        expected(__unexpect_inv, _Fn&& __fn)
        : _M_unex(std::forward<_Fn>(__fn)()), _M_has_value(false)
      { }

      union {
        _Tp _M_val;
        _Er _M_unex;
      };

      bool _M_has_value;
    };

    // Partial specialization for std::expected<cv void, E>
    template<typename _Tp, typename _Er> requires is_void_v<_Tp>
    class expected<_Tp, _Er>
    {
      static_assert( __expected::__can_be_unexpected<_Er> );

      template<typename _Up, typename _Err, typename _Unex = unexpected<_Er>>
      static constexpr bool __cons_from_expected
        = __or_v<is_constructible<_Unex, expected<_Up, _Err>&>,
                 is_constructible<_Unex, expected<_Up, _Err>>,
                 is_constructible<_Unex, const expected<_Up, _Err>&>,
                 is_constructible<_Unex, const expected<_Up, _Err>>
                 >;

      template<typename _Up>
      static constexpr bool __same_val
        = is_same_v<typename _Up::value_type, _Tp>;

      template<typename _Up>
      static constexpr bool __same_err
        = is_same_v<typename _Up::error_type, _Er>;

     public:
      using value_type = _Tp;
      using error_type = _Er;
      using unexpected_type = unexpected<_Er>;

      template<typename _Up>
      using rebind = expected<_Up, error_type>;

      constexpr
        expected() noexcept
        : _M_void(), _M_has_value(true)
      { }

      expected(const expected&) = default;

      constexpr
        expected(const expected& __x)
          noexcept(is_nothrow_copy_constructible_v<_Er>)
        requires is_copy_constructible_v<_Er>
                   && (!is_trivially_copy_constructible_v<_Er>)
        : _M_void(), _M_has_value(__x._M_has_value)
      {
        if (!_M_has_value)
          std::construct_at(__builtin_addressof(_M_unex), __x._M_unex);
      }

      expected(expected&&) = default;

      constexpr
        expected(expected&& __x)
          noexcept(is_nothrow_move_constructible_v<_Er>)
        requires is_move_constructible_v<_Er>
                   && (!is_trivially_move_constructible_v<_Er>)
        : _M_void(), _M_has_value(__x._M_has_value)
      {
        if (!_M_has_value)
          std::construct_at(__builtin_addressof(_M_unex),
                            std::move(__x)._M_unex);
      }

      template<typename _Up, typename _Gr>
        requires is_void_v<_Up>
                   && is_constructible_v<_Er, const _Gr&>
                   && (!__cons_from_expected<_Up, _Gr>)
      constexpr explicit(!is_convertible_v<const _Gr&, _Er>)
        expected(const expected<_Up, _Gr>& __x)
          noexcept(is_nothrow_constructible_v<_Er, const _Gr&>)
        : _M_void(), _M_has_value(__x._M_has_value)
      {
        if (!_M_has_value)
          std::construct_at(__builtin_addressof(_M_unex), __x._M_unex);
      }

      template<typename _Up, typename _Gr>
        requires is_void_v<_Up>
                   && is_constructible_v<_Er, _Gr>
                   && (!__cons_from_expected<_Up, _Gr>)
      constexpr explicit(!is_convertible_v<_Gr, _Er>)
        expected(expected<_Up, _Gr>&& __x)
          noexcept(is_nothrow_constructible_v<_Er, _Gr>)
        : _M_void(), _M_has_value(__x._M_has_value)
      {
        if (!_M_has_value)
          std::construct_at(__builtin_addressof(_M_unex),
                            std::move(__x)._M_unex);
      }

      template<typename _Gr = _Er>
        requires is_constructible_v<_Er, const _Gr&>
      constexpr explicit(!is_convertible_v<const _Gr&, _Er>)
        expected(const unexpected<_Gr>& __u)
          noexcept(is_nothrow_constructible_v<_Er, const _Gr&>)
        : _M_unex(__u.error()), _M_has_value(false)
      { }

      template<typename _Gr = _Er>
        requires is_constructible_v<_Er, _Gr>
      constexpr explicit(!is_convertible_v<_Gr, _Er>)
        expected(unexpected<_Gr>&& __u)
          noexcept(is_nothrow_constructible_v<_Er, _Gr>)
        : _M_unex(std::move(__u).error()), _M_has_value(false)
      { }

      constexpr explicit
        expected(in_place_t) noexcept
        : expected()
      { }

      template<typename... _Args>
        requires is_constructible_v<_Er, _Args...>
      constexpr explicit
        expected(unexpect_t, _Args&&... __args)
          noexcept(is_nothrow_constructible_v<_Er, _Args...>)
        : _M_unex(std::forward<_Args>(__args)...), _M_has_value(false)
      { }

      template<typename _Up, typename... _Args>
        requires is_constructible_v<_Er, initializer_list<_Up>&, _Args...>
      constexpr explicit
        expected(unexpect_t, initializer_list<_Up> __il, _Args&&... __args)
          noexcept(is_nothrow_constructible_v<_Er, initializer_list<_Up>&,
                                              _Args...>)
        : _M_unex(__il, std::forward<_Args>(__args)...), _M_has_value(false)
      { }

      constexpr ~expected() = default;

      constexpr ~expected() requires (!is_trivially_destructible_v<_Er>)
      {
        if (!_M_has_value)
          std::destroy_at(__builtin_addressof(_M_unex));
      }

      // assignment

      expected& operator=(const expected&) = delete;

      constexpr expected&
      operator=(const expected& __x)
        noexcept(__and_v<is_nothrow_copy_constructible<_Er>,
                         is_nothrow_copy_assignable<_Er>>)
        requires is_copy_constructible_v<_Er>
                 && is_copy_assignable_v<_Er>
      {
        if (__x._M_has_value)
          emplace();
        else
          _M_assign_unex(__x._M_unex);
        return *this;
      }

      constexpr expected&
      operator=(expected&& __x)
        noexcept(__and_v<is_nothrow_move_constructible<_Er>,
                         is_nothrow_move_assignable<_Er>>)
        requires is_move_constructible_v<_Er>
                 && is_move_assignable_v<_Er>
      {
        if (__x._M_has_value)
          emplace();
        else
          _M_assign_unex(std::move(__x._M_unex));
        return *this;
      }

      template<typename _Gr>
        requires is_constructible_v<_Er, const _Gr&>
                 && is_assignable_v<_Er&, const _Gr&>
      constexpr expected&
      operator=(const unexpected<_Gr>& __e)
      {
        _M_assign_unex(__e.error());
        return *this;
      }

      template<typename _Gr>
        requires is_constructible_v<_Er, _Gr>
                 && is_assignable_v<_Er&, _Gr>
      constexpr expected&
      operator=(unexpected<_Gr>&& __e)
      {
        _M_assign_unex(std::move(__e.error()));
        return *this;
      }

      // modifiers

      constexpr void
      emplace() noexcept
      {
        if (!_M_has_value)
        {
          std::destroy_at(__builtin_addressof(_M_unex));
          _M_has_value = true;
        }
      }

      // swap
      constexpr void
      swap(expected& __x)
        noexcept(__and_v<is_nothrow_swappable<_Er&>,
                         is_nothrow_move_constructible<_Er>>)
        requires is_swappable_v<_Er> && is_move_constructible_v<_Er>
      {
        if (_M_has_value)
        {
          if (!__x._M_has_value)
          {
            std::construct_at(__builtin_addressof(_M_unex),
                              std::move(__x._M_unex)); // might throw
            std::destroy_at(__builtin_addressof(__x._M_unex));
            _M_has_value = false;
            __x._M_has_value = true;
          }
        }
        else
        {
          if (__x._M_has_value)
          {
            std::construct_at(__builtin_addressof(__x._M_unex),
                              std::move(_M_unex)); // might throw
            std::destroy_at(__builtin_addressof(_M_unex));
            _M_has_value = true;
            __x._M_has_value = false;
          }
          else
          {
            using std::swap;
            swap(_M_unex, __x._M_unex);
          }
        }
      }

      // observers

      [[nodiscard]]
      constexpr explicit
      operator bool() const noexcept { return _M_has_value; }

      [[nodiscard]]
      constexpr bool has_value() const noexcept { return _M_has_value; }

      constexpr void
      operator*() const noexcept { __glibcxx_assert(_M_has_value); }

      constexpr void
      value() const&
      {
        if (_M_has_value) [[likely]]
          return;
        _GLIBCXX_THROW_OR_ABORT(bad_expected_access<_Er>(_M_unex));
      }

      constexpr void
      value() &&
      {
        if (_M_has_value) [[likely]]
          return;
        _GLIBCXX_THROW_OR_ABORT(bad_expected_access<_Er>(std::move(_M_unex)));
      }

      constexpr const _Er&
      error() const & noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return _M_unex;
      }

      constexpr _Er&
      error() & noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return _M_unex;
      }

      constexpr const _Er&&
      error() const && noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return std::move(_M_unex);
      }

      constexpr _Er&&
      error() && noexcept
      {
        __glibcxx_assert(!_M_has_value);
        return std::move(_M_unex);
      }

      template<typename _Gr = _Er>
      constexpr _Er
      error_or(_Gr&& __e) const&
      {
        static_assert( is_copy_constructible_v<_Er> );
        static_assert( is_convertible_v<_Gr, _Er> );

        if (_M_has_value)
          return std::forward<_Gr>(__e);
        return _M_unex;
      }

      template<typename _Gr = _Er>
      constexpr _Er
      error_or(_Gr&& __e) &&
      {
        static_assert( is_move_constructible_v<_Er> );
        static_assert( is_convertible_v<_Gr, _Er> );

        if (_M_has_value)
          return std::forward<_Gr>(__e);
        return std::move(_M_unex);
      }

      // monadic operations

      template<typename _Fn> requires is_constructible_v<_Er, _Er&>
      constexpr auto
      and_then(_Fn&& __f) &
      {
        using _Up = __expected::__result0<_Fn>;
        static_assert(__expected::__is_expected<_Up>);
        static_assert(is_same_v<typename _Up::error_type, _Er>);

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f));
        else
          return _Up(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er&>
      constexpr auto
      and_then(_Fn&& __f) const &
      {
        using _Up = __expected::__result0<_Fn>;
        static_assert(__expected::__is_expected<_Up>);
        static_assert(is_same_v<typename _Up::error_type, _Er>);

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f));
        else
          return _Up(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, _Er>
      constexpr auto
      and_then(_Fn&& __f) &&
      {
        using _Up = __expected::__result0<_Fn>;
        static_assert(__expected::__is_expected<_Up>);
        static_assert(is_same_v<typename _Up::error_type, _Er>);

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f));
        else
          return _Up(unexpect, std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er>
      constexpr auto
      and_then(_Fn&& __f) const &&
      {
        using _Up = __expected::__result0<_Fn>;
        static_assert(__expected::__is_expected<_Up>);
        static_assert(is_same_v<typename _Up::error_type, _Er>);

        if (has_value())
          return std::__invoke(std::forward<_Fn>(__f));
        else
          return _Up(unexpect, std::move(_M_unex));
      }

      template<typename _Fn>
      constexpr auto
      or_else(_Fn&& __f) &
      {
        using _Gr = __expected::__result<_Fn, _Er&>;
        static_assert(__expected::__is_expected<_Gr>);
        static_assert(is_same_v<typename _Gr::value_type, _Tp>);

        if (has_value())
          return _Gr();
        else
          return std::__invoke(std::forward<_Fn>(__f), _M_unex);
      }

      template<typename _Fn>
      constexpr auto
      or_else(_Fn&& __f) const &
      {
        using _Gr = __expected::__result<_Fn, const _Er&>;
        static_assert(__expected::__is_expected<_Gr>);
        static_assert(is_same_v<typename _Gr::value_type, _Tp>);

        if (has_value())
          return _Gr();
        else
          return std::__invoke(std::forward<_Fn>(__f), _M_unex);
      }

      template<typename _Fn>
      constexpr auto
      or_else(_Fn&& __f) &&
      {
        using _Gr = __expected::__result<_Fn, _Er&&>;
        static_assert(__expected::__is_expected<_Gr>);
        static_assert(is_same_v<typename _Gr::value_type, _Tp>);

        if (has_value())
          return _Gr();
        else
          return std::__invoke(std::forward<_Fn>(__f), std::move(_M_unex));
      }

      template<typename _Fn>
      constexpr auto
      or_else(_Fn&& __f) const &&
      {
        using _Gr = __expected::__result<_Fn, const _Er&&>;
        static_assert(__expected::__is_expected<_Gr>);
        static_assert(is_same_v<typename _Gr::value_type, _Tp>);

        if (has_value())
          return _Gr();
        else
          return std::__invoke(std::forward<_Fn>(__f), std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Er, _Er&>
      constexpr auto
      transform(_Fn&& __f) &
      {
        using _Up = __expected::__result0_xform<_Fn>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
        else
          return _Res(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er&>
      constexpr auto
      transform(_Fn&& __f) const &
      {
        using _Up = __expected::__result0_xform<_Fn>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
        else
          return _Res(unexpect, _M_unex);
      }

      template<typename _Fn> requires is_constructible_v<_Er, _Er>
      constexpr auto
      transform(_Fn&& __f) &&
      {
        using _Up = __expected::__result0_xform<_Fn>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
        else
          return _Res(unexpect, std::move(_M_unex));
      }

      template<typename _Fn> requires is_constructible_v<_Er, const _Er>
      constexpr auto
      transform(_Fn&& __f) const &&
      {
        using _Up = __expected::__result0_xform<_Fn>;
        using _Res = expected<_Up, _Er>;

        if (has_value())
          return _Res(__in_place_inv{}, std::forward<_Fn>(__f));
        else
          return _Res(unexpect, std::move(_M_unex));
      }

      template<typename _Fn>
      constexpr auto
      transform_error(_Fn&& __f) &
      {
        using _Gr = __expected::__result_xform<_Fn, _Er&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res();
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 _M_unex);
          });
      }

      template<typename _Fn>
      constexpr auto
      transform_error(_Fn&& __f) const &
      {
        using _Gr = __expected::__result_xform<_Fn, const _Er&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res();
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 _M_unex);
          });
      }

      template<typename _Fn>
      constexpr auto
      transform_error(_Fn&& __f) &&
      {
        using _Gr = __expected::__result_xform<_Fn, _Er&&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res();
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 std::move(_M_unex));
          });
      }

      template<typename _Fn>
      constexpr auto
      transform_error(_Fn&& __f) const &&
      {
        using _Gr = __expected::__result_xform<_Fn, const _Er&&>;
        using _Res = expected<_Tp, _Gr>;

        if (has_value())
          return _Res();
        else
          return _Res(__unexpect_inv{}, [&]() {
            return std::__invoke(std::forward<_Fn>(__f),
                                 std::move(_M_unex));
          });
      }

      // equality operators

      template<typename _Up, typename _Er2>
        requires is_void_v<_Up>
      friend constexpr bool
      operator==(const expected& __x, const expected<_Up, _Er2>& __y)
      // FIXME: noexcept(noexcept(bool(__x.error() == __y.error())))
      {
        if (__x.has_value())
          return __y.has_value();
        else
          return !__y.has_value() && bool(__x.error() == __y.error());
      }

      template<typename _Er2>
      friend constexpr bool
      operator==(const expected& __x, const unexpected<_Er2>& __e)
      // FIXME: noexcept(noexcept(bool(__x.error() == __e.error())))
      { return !__x.has_value() && bool(__x.error() == __e.error()); }

      friend constexpr void
      swap(expected& __x, expected& __y)
        noexcept(noexcept(__x.swap(__y)))
        requires requires { __x.swap(__y); }
      { __x.swap(__y); }

     private:
      template<typename, typename> friend class expected;

      template<typename _Vp>
      constexpr void
      _M_assign_unex(_Vp&& __v)
      {
        if (_M_has_value)
        {
          std::construct_at(__builtin_addressof(_M_unex),
                            std::forward<_Vp>(__v));
          _M_has_value = false;
        }
        else
          _M_unex = std::forward<_Vp>(__v);
      }

      using __in_place_inv = __expected::__in_place_inv;
      using __unexpect_inv = __expected::__unexpect_inv;

      template<typename _Fn>
      explicit constexpr
        expected(__in_place_inv, _Fn&& __fn)
        : _M_void(), _M_has_value(true)
      { std::forward<_Fn>(__fn)(); }

      template <typename F>
      explicit constexpr expected(__unexpect_inv, _Fn&& __fn)
        : _M_unex(std::forward<_Fn>(__fn)()), _M_has_value(false)
      {}

      union {
        T m_val;
        E m_unex;
      };
      bool m_has_value;
    };
  }
}