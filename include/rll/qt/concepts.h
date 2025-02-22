#pragma once

#include <rll/global/definitions.h>

#if defined(RLL_QT_CORE) || defined(RLL_DOC)
#  include <qobject.h>
#  include <type_traits>
#  if __cplusplus >= 202'002L
#    include <concepts>
#  endif

namespace rll {
#  if __cplusplus >= 202'002L || defined(RLL_DOC)
  template <typename T>
  concept qobject = std::is_base_of_v<::QObject, T>;

  template <typename T>
  concept qgadget = requires(T t) {
    { T::staticMetaObject } -> std::convertible_to<::QMetaObject>;
  };
#  endif

  template <typename T>
  struct is_qobject : std::integral_constant<bool, std::is_base_of_v<::QObject, T>> {};

  template <typename T>
  inline constexpr bool is_qobject_v = is_qobject<T>::value;

  template <typename T, typename = void>
  struct is_qgadget : std::false_type {};

  template <typename T>
  struct is_qgadget<T, std::void_t<decltype(T::staticMetaObject)>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_qgadget_v = is_qgadget<T>::value;
}  // namespace rll
#endif
