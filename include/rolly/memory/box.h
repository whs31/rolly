#pragma once

#include <memory>
#include "../contracts.h"
#include "../traits/noncopyable.h"
#include "../concepts/ptr.h"
#include "../types/optional.h"

namespace rolly {
  /**
   * @brief Owning unique smart pointer which can never be null.
   * @details The box class behaves very similarly to the <code>std::unique_ptr</code> class, but
   * unlike the <code>std::unique_ptr</code>, it is 100% guaranteed to be filled at all times
   * (unless the user tries to access it after calling std::move does something else that is clearly
   * a bad idea).
   *
   * Reference implementation is taken from the <b>reflect-cpp</b> library.
   *
   * API reference matches (almost) the Rust Language implementation.
   * @tparam T Type of data stored in the box. Must fulfill <code>std::destructible</code>
   * requirement.
   * @sa https://github.com/getml/reflect-cpp/blob/main/include/rfl/Box.hpp
   * @sa https://doc.rust-lang.org/std/boxed/struct.Box.html
   */
  template <typename T>
  class box : noncopyable  // NOLINT(*-special-member-functions)
  {
   public:
    using value_type = T;
    using element_type = T;

    /**
     * @brief Creates an box with default-constructed data of type <code>T</code>.
     */
    box()
      : ptr_(std::make_unique<T>()) {}

    /**
     * @brief Default destructor.
     */
    ~box() = default;

    /**
     * @brief Default move constructor.
     */
    box(box&&) noexcept = default;

    /**
     * @brief Moves data from another box.
     * @details Takes ownership of the other box's data.
     * @tparam U Underlying type of the other box.
     * @param other Other box to move data from.
     */
    template <typename U>
    box(box<U>&& other
    ) noexcept  // NOLINT(*-explicit-constructor, *-rvalue-reference-param-not-moved)
      : ptr_(std::forward<std::unique_ptr<U>>(other.as_unique_ptr())) {}

    /**
     * @brief Moves data from another box.
     * @details Takes ownership of the other box's data.
     * @tparam U Underlying type of the other box.
     * @param other Other box to move data from.
     * @returns Reference to this box.
     */
    template <typename U>
    box<T>& operator=(box<U>&& other) noexcept  // NOLINT(*-rvalue-reference-param-not-moved)
    {
      this->ptr_ = std::forward<std::unique_ptr<U>>(other.as_unique_ptr());
      return *this;
    }

    /**
     * @brief Returns a mutable pointer to the underlying data.
     * @returns Mutable pointer to the underlying data.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T* get() const {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return this->ptr_.get();
    }

    /**
     * @brief Returns a mutable pointer to the underlying data.
     * @returns Mutable pointer to the underlying data.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T* get() {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return this->ptr_.get();
    }

    /**
     * @brief Returns a mutable pointer to the underlying data.
     * @returns Mutable pointer to the underlying data.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T* ptr_mut() {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return this->ptr_.get();
    }

    /**
     * @brief Returns an immutable pointer to the underlying data.
     * @returns Immutable pointer to the underlying data.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T const* ptr() const {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return this->ptr_.get();
    }

    /**
     * @brief Returns a mutable reference to the underlying object.
     * @return A mutable reference to the underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T& ref_mut() {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return *this->ptr_;
    }

    /**
     * @brief Returns an immutable reference to the underlying object.
     * @return An immutable reference to the underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T const& ref() const {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return *this->ptr_;
    }

    /**
     * @brief Returns an mutable reference to the underlying object.
     * @return An mutable reference to the underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T& operator*() {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return *this->ptr_;
    }

    /**
     * @brief Returns an immutable reference to the underlying object.
     * @return An immutable reference to the underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T const& operator*() const {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return *this->ptr_;
    }

    /**
     * @brief Returns the underlying object.
     * @return Pointer to the underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T* operator->() {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return this->ptr_.get();
    }

    /**
     * @brief Returns the underlying object.
     * @return Pointer to the underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    [[nodiscard]] T* operator->() const {
      contracts::invariant(this->ptr_ != nullptr, "use after calling box::leak");
      return this->ptr_.get();
    }

    /**
     * @brief Returns the underlying unique pointer.
     */
    [[nodiscard]] std::unique_ptr<T>& as_unique_ptr() noexcept { return this->ptr_; }

    /**
     * @brief Returns the constant underlying unique pointer.
     */
    [[nodiscard]] std::unique_ptr<T> const& as_unique_ptr() const noexcept { return this->ptr_; }

    /**
     * @brief Consumes and leaks the box, returning a mutable pointer to the underlying object.
     * @details The caller is responsible for freeing the memory by calling deleter on the returned
     * pointer.
     * @note Calling this function leaves this box in an invalid state. Trying to use it afterwards
     * will result in contract violation.
     * @returns Mutable unmanaged pointer to the underlying object or <code>nullptr</code> if the
     * box is already consumed.
     */
    [[nodiscard]] T* leak() noexcept { return this->ptr_.release(); }

    /**
     * @brief Tries to downcast the underlying pointer to the given type.
     * @tparam U Type to cast to.
     * @return An optional containing a reference to the casted underlying object if successful,
     * <code>std::nullopt</code> otherwise.
     * @invariant <code>box::leak</code> was not called before.
     */
    template <typename U>
    [[nodiscard]] optional<std::reference_wrapper<U>> downcast() {
      contracts::invariant(this->ptr_ != nullptr, "use after consume");
      try {
        auto& r = dynamic_cast<U&>(this->ref_mut());
        return rolly::make_optional(std::ref(r));
      } catch(std::bad_cast const&) {  // NOLINT(*-empty-catch)
        return nullopt;
      } catch(...) {                   // NOLINT(*-empty-catch)
        return nullopt;
      }
    }

    /**
     * @brief Casts the underlying pointer to the given type.
     * @tparam U Type to cast to.
     * @return A pointer to the casted underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    template <typename U>
    [[nodiscard]] U* as() {
      contracts::invariant(this->ptr_ != nullptr, "use after consume");
      return static_cast<U*>(this->ptr_mut());
    }

    /**
     * @brief Casts the underlying pointer to the immutable given type.
     * @tparam U Type to cast to.
     * @return An constant pointer to the casted underlying object.
     * @invariant <code>box::leak</code> was not called before.
     */
    template <typename U>
    [[nodiscard]] U const* as() const {
      contracts::invariant(this->ptr_ != nullptr, "use after consume");
      return static_cast<U*>(this->ptr());
    }

    /**
     * @brief Returns <code>true</code> if the box is not consumed.
     */
    [[nodiscard]] explicit operator bool() const noexcept { return this->ptr_ != nullptr; }

    /**
     * @brief Creates a new box from given arguments.
     * @tparam Args Constructor arguments type.
     * @param args Constructor arguments.
     * @return <tt>box</tt> constructed from <tt>args</tt>.
     */
    template <typename... Args>
    [[nodiscard]] static box<T> make(Args&&... args) {
      return box<T>(std::make_unique<T>(std::forward<Args>(args)...));
    }

    /**
     * @brief Creates a new box from <tt>std::unique_ptr</tt>.
     * @param ptr <tt>std::unique_ptr</tt> to construct from.
     * @pre <tt>ptr</tt> must not be null.
     * @return <tt>box</tt> constructed from <tt>ptr</tt>.
     */
    [[nodiscard]] static box<T> make(std::unique_ptr<T> ptr) noexcept(false) {
      contracts::precondition(ptr != nullptr, "ptr must not be null");
      return box<T>(std::move(ptr));
    }

   private:
    explicit box(std::unique_ptr<T>&& _ptr) noexcept
      : ptr_(std::move(_ptr)) {}

    std::unique_ptr<T> ptr_;
  };

  /**
   * @brief Creates a new box with the given arguments.
   * @tparam T Type of the box underlying data.
   * @tparam Args Constructor arguments type.
   * @param args Arguments to the constructor.
   * @return A new box with the given arguments.
   * @sa http://en.cppreference.com/w/cpp/memory/make_unique
   * @relates box
   */
  template <typename T, typename... Args>
  [[nodiscard]] box<T> make_box(Args&&... args) {
    return box<T>::make(std::forward<Args>(args)...);
  }

#if defined(___rolly_cxx20___)
  /**
   * @brief Compares two boxes.
   * @tparam T1 Underlying type of the first box.
   * @tparam T2 Underlying type of the second box.
   * @param a First box.
   * @param b Second box.
   * @return Comparison result.
   * @relates box
   * @note Available only in C++20 mode.
   */
  template <typename T1, typename T2>
  auto operator<=>(box<T1> const& a, box<T2> const& b) {
    return a.as_unique_ptr() <=> b.as_unique_ptr();
  }

  static_assert(concepts::smart_ptr<box<int>>);
  static_assert(concepts::ptr_like<box<int>>);
#endif  // ___rolly_cxx20___
}  // namespace rolly

namespace std {
  /**
   * @brief Hashes a <tt>box</tt>.
   * @tparam T Underlying type of the <tt>box</tt>.
   * @param b <tt>box</tt> to hash.
   * @return Hash value.
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <typename T>
  struct [[maybe_unused]] hash<rolly::box<T>> {
    [[nodiscard]] std::size_t operator()(rolly::box<T> const& b) const {
      return std::hash<std::unique_ptr<T>>()(b.as_unique_ptr());
    }
  };

  /**
   * @brief Swaps two <tt>box</tt>es.
   * @tparam T Underlying type of the <tt>box</tt>es.
   * @param a First <tt>box</tt>.
   * @param b Second <tt>box</tt>.
   * @sa http://en.cppreference.com/w/cpp/memory/swap
   * @relates rolly::box
   */
  template <typename T>
  [[maybe_unused]] void swap(rolly::box<T>& a, rolly::box<T>& b) noexcept {
    return std::swap(a.as_unique_ptr(), b.as_unique_ptr());
  }
}  // namespace std
