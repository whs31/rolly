#pragma once

#include <memory>
#include <compare>
#include <stdexcept>
#include <floppy/detail/export.h>
#include <floppy/detail/formatters.h>

namespace floppy
{
  /// \brief Exception thrown when trying to access invalid smart pointer.
  /// \details Can be thrown when accessing a <tt>box</tt> that has been moved from or is leaked/consumed.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  class invalid_smart_pointer_access : public std::logic_error
  {
    using std::logic_error::logic_error;
    using std::logic_error::what;
  };

  /// \brief Box memory class.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \details The box class behaves very similarly to the unique_ptr, but unlike the
  /// unique_ptr, it is 100% guaranteed to be filled at all times (unless the user
  /// tries to access it after calling std::move does something else that is
  /// clearly bad practice).
  ///
  /// Reference implementation is taken from the <b>reflect-cpp</b> library.
  /// API reference matches (almost) the Rust Language implementation.
  /// \tparam T Type of data stored in the box.
  /// \sa https://github.com/getml/reflect-cpp/blob/main/include/rfl/Box.hpp
  /// \sa https://doc.rust-lang.org/std/boxed/struct.Box.html
  template <typename T>
  class box
  {
   public:
    /// \brief Default constructor.
    box() : ptr_(std::make_unique<T>()) {} // NOLINT(*-use-equals-default)

    /// \brief Copy constructor (deleted).
    box(box const& other) = delete;

    /// \brief Move constructor.
    box(box&& other) noexcept = default;

    /// \brief Move constructor.
    /// \details Takes ownership of <tt>other</tt>
    /// \tparam U Underlying type of <tt>other</tt>.
    /// \param other <tt>box</tt> to move from.
    template <typename U>
    box(box<U>&& other) noexcept // NOLINT(*-explicit-constructor, *-rvalue-reference-param-not-moved)
      : ptr_(std::forward<std::unique_ptr<U>>(other.as_unique_ptr()))
    {}

    /// \brief Default destructor.
    ~box() = default;

    /// \brief Returns a mutable pointer to the underlying object.
    /// \returns A mutable pointer to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto get() -> T* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::get: use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns a mutable pointer to the underlying object.
    /// \returns A mutable pointer to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto get() const -> T* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::get: use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns a mutable pointer to the underlying object.
    /// \returns A mutable pointer to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto ptr_mut() -> T* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::ptr_mut: use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns an immutable pointer to the underlying object.
    /// \returns An immutable pointer to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto ptr() const -> T const* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::ptr: use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns a mutable reference to the underlying object.
    /// \returns A mutable reference to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto ref_mut() -> T& {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::ref_mut: use after consume");
      return *this->ptr_;
    }

    /// \brief Returns an immutable reference to the underlying object.
    /// \returns An immutable reference to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto ref() const -> T const& {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::ref: use after consume");
      return *this->ptr_;
    }

    /// \brief Returns a mutable reference to the underlying object.
    /// \returns A mutable reference to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto operator*() -> T& {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::operator*: use after consume");
      return *this->ptr_;
    }

    /// \brief Returns an immutable reference to the underlying object.
    /// \returns An immutable reference to the underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto operator*() const -> T const& {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::operator->: use after consume");
      return *this->ptr_;
    }

    /// \brief Copy assignment operator (deleted).
    auto operator=(box<T> const& other) -> box<T>& = delete;

    /// \brief Move assignment operator.
    auto operator=(box<T>&& other) noexcept -> box<T>& = default;

    /// \brief Move assignment operator.
    /// \details Takes ownership of <tt>other</tt>
    /// \tparam U Underlying type of <tt>other</tt>.
    /// \param other <tt>box</tt> to move from.
    /// \returns A reference to <tt>*this</tt>.
    template <typename U>
    auto operator=(box<U>&& other) noexcept -> box<T>& // NOLINT(*-rvalue-reference-param-not-moved)
    {
      this->ptr_ = std::forward<std::unique_ptr<U>>(other.as_unique_ptr());
      return *this;
    }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto operator->() -> T* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::operator->: use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    [[nodiscard]] auto operator->() const -> T* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::operator->: use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns the underlying unique pointer.
    /// \returns The underlying unique pointer.
    [[nodiscard]] auto as_unique_ptr() noexcept -> std::unique_ptr<T>& { return this->ptr_; }

    /// \brief Returns the constant underlying unique pointer.
    /// \returns The underlying unique pointer.
    [[nodiscard]] auto as_unique_ptr() const noexcept -> std::unique_ptr<T> const& { return this->ptr_; }

    /// \brief Consumes and leaks the box, returning a mutable pointer to the underlying object.
    /// \details The caller is responsible for deleting the returned pointer.
    /// Calling this function leaves the <tt>box</tt> in an invalid state.
    /// \returns A mutable unmanaged pointer to the underlying object.
    [[nodiscard]] auto leak() noexcept -> T* {
      return this->ptr_.release();
    }

    /// \brief Attempt to downcast the box to a concrete type.
    /// \tparam U Type to downcast to.
    /// \returns An mutable reference to the casted underlying object if successful, <tt>none</tt> otherwise.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    template <typename U>
    [[nodiscard]] auto downcast() -> option<fl::types::ref<U>> {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::downcast: use after consume");
      try {
        auto& r = dynamic_cast<U&>(this->ref_mut());
        return option<fl::types::ref<U>>{r};
      } catch(std::bad_cast const&) {
        return none;
      } catch(...) {
        return none;
      }
    }

    /// \brief Casts the underlying pointer to given type.
    /// \tparam U Type to cast to.
    /// \returns An mutable pointer to the casted underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    template <typename U>
    [[nodiscard]] auto as() -> U* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::as: use after consume");
      return static_cast<U*>(this->ptr_mut());
    }

    /// \brief Casts the underlying pointer to constant given type.
    /// \tparam U Type to cast to.
    /// \returns An constant pointer to the casted underlying object.
    /// \throws invalid_smart_pointer_access If the box has been moved from or is leaked/consumed.
    template <typename U>
    [[nodiscard]] auto as() const -> U const* {
      if(not this->ptr_)
        throw invalid_smart_pointer_access("box::as: use after consume");
      return static_cast<U*>(this->ptr());
    }

    /// \brief Constructs new box with given arguments.
    /// \param Args Constructor arguments type.
    /// \param args Constructor arguments.
    template <typename... Args>
    [[nodiscard]] static auto make(Args&&... args) -> box<T> {
      return box<T>(std::make_unique<T>(std::forward<Args>(args)...));
    }

    /// \brief Tries to construct new box from <tt>std::unique_ptr</tt>.
    /// \param ptr <tt>std::unique_ptr</tt> to construct from.
    /// \throws std::invalid_argument if <tt>ptr</tt> is not set.
    [[nodiscard]] static auto make(std::unique_ptr<T> ptr) noexcept(false) -> box<T> {
       if(not ptr)
         throw std::invalid_argument("box::make: ptr is not set");
       return box<T>(std::move(ptr));
    }

   private:
    explicit box(std::unique_ptr<T>&& _ptr) noexcept : ptr_(std::move(_ptr)) {}

   private:
    std::unique_ptr<T> ptr_;
  };

  /// \brief Constructs a new box with given arguments.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  template <typename T, typename... Args>
  [[nodiscard]] auto make_box(Args&&... args) -> box<T> {
    return box<T>::make(std::forward<Args>(args)...);
  }

  /// \brief Compares two boxes.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \param a First box.
  /// \param b Second box.
  /// \returns The result of the comparison.
  template <typename T1, typename T2>
  auto operator<=>(box<T1> const& a, box<T2> const& b) {
    return a.as_unique_ptr() <=> b.as_unique_ptr();
  }

  /// \brief Prints a <tt>box</tt> to an output stream.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \param os Output stream.
  /// \param b <tt>box</tt> to print.
  template <typename C, typename Traits, typename T>
  auto operator<<(std::basic_ostream<C, Traits>& os, box<T> const& b) -> std::basic_ostream<C, Traits>& {
    return os << *b;
  }
} // namespace floppy

namespace std
{
  /// \brief Hashes a <tt>box</tt>.
  template <typename T>
  struct [[maybe_unused]] hash<floppy::box<T>>
  {
    [[nodiscard]] auto operator()(floppy::box<T> const& b) const -> std::size_t {
      return std::hash<std::unique_ptr<T>>()(b.as_unique_ptr());
    }
  };

  /// \brief Swaps two boxes.
  template <typename T>
  [[maybe_unused]] auto swap(floppy::box<T>& a, floppy::box<T>& b) noexcept -> void {
    return std::swap(a.as_unique_ptr(), b.as_unique_ptr());
  }
} // namespace std

/// \defgroup memory Memory
/// \ingroup foundation
/// \brief Memory related classes and functions.