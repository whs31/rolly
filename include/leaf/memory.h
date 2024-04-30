#pragma once

#include <new>
#include <memory>
#include <leaf/traits.h>
#include <leaf/utils/noexcept.h>

namespace leaf
{
  using leaf::utils::make_unique_nothrow;
  using leaf::utils::make_shared_nothrow;

  /**
   * \brief Принимает сырой указатель на объект и возвращает владеющий std::unique_ptr на исходный объект.
   * \tparam T Указатель на объект. Не может быть массивом: `delete T` должно быть синтаксически правильным выражением.
   * \sa https://github.com/abseil/abseil-cpp/blob/master/absl/memory/memory.h
   */
  template <typename T>
  requires (not traits::Array<T>) and traits::Object<T>
  [[nodiscard("do not discard result of make_unique or wrap_unique calls")]]
  auto wrap_unique(T* ptr) noexcept(noexcept(make_unique_nothrow<T>(ptr))) -> std::unique_ptr<T>
  {
    return make_unique_nothrow<T>(ptr);
  }

  template <typename T, typename D>
  [[nodiscard("do not discard result of make_shared_from_unique call")]]
  auto make_shared_from_unique(std::unique_ptr<T, D>&& ptr) noexcept(false) -> std::shared_ptr<T>
  {
    return ptr ? std::shared_ptr<T>(std::move(ptr)) : std::shared_ptr<T>();
  }
}