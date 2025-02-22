#pragma once

#include <list>
#include <rll/qt/formatters.h>
#include <rll/qt/literals.h>
#include <rll/types/optional.h>
#include <rll/types/stdint.h>

#ifdef _MSC_VER
#  include <ciso646>
#endif

#if defined(RLL_QT_GUI) || defined(RLL_DOC)
#  include <algorithm>
#  include <stdexcept>
#  include <string>
#  include <string_view>
#  include <qobject.h>
#  include <qqml.h>
#  include <qqmlengine.h>
#  include <fmt/format.h>
#  include <rll/global/version.h>
#  include <rll/qt/concepts.h>

namespace rll::qt::qml {
  namespace detail {
    enum class strip_kind : u8 {
      prefix_and_extension,
      namespace_,  // NOLINT(*-identifier-naming)
      hungarian_prefix
    };

    [[nodiscard]] inline std::string strip(std::string_view str, strip_kind kind) {
      if(kind == strip_kind::prefix_and_extension) {
        auto const last_slash = str.find_last_of('/');
        auto const last_dot = str.find_last_of('.');
        if(last_slash == std::string_view::npos or last_dot == std::string_view::npos)
          throw std::invalid_argument(fmt::format("rolly::qt::qml::strip: invalid url ({})", str));
        return std::string(str.substr(last_slash + 1, last_dot - last_slash - 1));
      }
      if(kind == strip_kind::namespace_) {
        auto const last_colon = str.find_last_of(':');
        if(last_colon == std::string_view::npos)
          return std::string(str);
        return std::string(str.substr(last_colon + 1));
      }
      if(kind == strip_kind::hungarian_prefix) {
        if(str.size() < 2)
          return std::string(str);
        if(str.front() == 'C' and std::isupper(str.at(1)))
          return std::string(str.substr(1));
        return std::string(str);
      }
      return std::string(str);
    }
  }  // namespace detail

  class module {
   public:
    using version_type = version;

    explicit module(std::string name, version_type version = {1, 0, 0}) noexcept
      : name_(std::move(name))
      , version_(version) {
      ::qmlRegisterModule(this->name_.c_str(), this->version_.major, this->version_.minor);
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<::QObject, T>>>
    module& component(optional<std::string_view> name = nullopt) {
      auto const component_name = module::demangle_class_name<T>(name);
      ::qmlRegisterType<T>(
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str()
      );
      return *this;
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<::QObject, T>>>
    module& singleton(T* instance, optional<std::string_view> name = nullopt) {
      auto const component_name = module::demangle_class_name<T>(name);
      ::qmlRegisterSingletonInstance(
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str(),
        instance
      );
      return *this;
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<::QObject, T>>>
    module& singleton(optional<std::string_view> name = nullopt) {
      auto const component_name = module::demangle_class_name<T>(name);
      ::qmlRegisterSingletonType<T>(
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str(),
        T::create
      );
      return *this;
    }

    module& file(std::string_view url, optional<std::string_view> name = nullopt) {
      auto const component_name = module::demangle_file_url(url, name);
      ::qmlRegisterType(
        ::QUrl(url.data()),  // NOLINT(*-suspicious-stringview-data-usage)
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str()
      );
      return *this;
    }

    template <typename T, typename = std::enable_if_t<is_qgadget_v<T>>>
    module& uncreatable(
      optional<std::string_view> name = nullopt,
      optional<std::string_view> reason = nullopt
    ) {
      auto const component_name = module::demangle_class_name<T>(name);
      auto const reason_string = [&]() -> std::string {
        if(not reason.has_value())
          return fmt::format("Class \'{}\' is uncreatable", component_name);
        return std::string(reason.value());
      }();
      ::qmlRegisterUncreatableType<T>(
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str(),
        ::QString::fromStdString(reason_string)
      );
      return *this;
    }

   private:
    template <typename T, typename = std::enable_if_t<is_qgadget_v<T>>>
    [[nodiscard]] static std::string demangle_class_name(optional<std::string_view> name) {
      if(name)
        return std::string(*name);
      auto const meta_name =
        detail::strip(T::staticMetaObject.className(), detail::strip_kind::namespace_);
      return detail::strip(meta_name, detail::strip_kind::hungarian_prefix);
    }

    [[nodiscard]] static std::string
      demangle_file_url(std::string_view const url, optional<std::string_view> name) {
      if(name)
        return std::string(*name);
      return detail::strip(url, detail::strip_kind::prefix_and_extension);
    }

    std::string name_;
    version_type version_;
  };
}  // namespace rll::qt::qml
#endif  // defined(RLL_QT_GUI)
