#pragma once

#include "../global/definitions.h"

#if defined(ROLLY_QT_CORE) || defined(ROLLY_DOC)
#  include <qobject.h>
#  include <type_traits>
#  ifdef ___rolly_cxx20___
#    include <concepts>
#  endif  // ___rolly_cxx20___

namespace rolly::concepts {
#  if defined(___rolly_cxx20___) || defined(ROLLY_DOC)
  template <typename T>
  concept qobject = std::is_base_of_v<::QObject, T>;

  template <typename T>
  concept qgadget = requires(T t) {
    { T::staticMetaObject } -> std::convertible_to<::QMetaObject>;
  };
#  endif
}  // namespace rolly::concepts

namespace rolly {
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
}  // namespace rolly
#endif
