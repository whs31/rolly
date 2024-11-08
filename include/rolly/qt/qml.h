#pragma once

#include <list>
#include "formatters.h"
#include "literals.h"

#if defined(ROLLY_QT_GUI) || defined(ROLLY_DOC)
#  include <algorithm>
#  include <stdexcept>
#  include <string>
#  include <string_view>
#  include <optional>
#  include <qobject.h>
#  include <qqml.h>
#  include <qqmlengine.h>
#  include <fmt/format.h>
#  include <fmt/color.h>
#  include "./registrable.h"

namespace rolly::qt::qml {
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

    explicit module(std::string name, version_type version = {0, 0, 0}) noexcept
      : name_(std::move(name))
      , version_(version) {
      if constexpr(implicit_verbosity == verbosity::verbose) {
        fmt::println(
          "rolly::qt::qml: registering module {} v{}.{}",
          fmt::styled(this->name_, fmt::fg(fmt::terminal_color::blue) | fmt::emphasis::bold),
          fmt::styled(this->version_.major, fmt::fg(fmt::terminal_color::cyan)),
          fmt::styled(this->version_.minor, fmt::fg(fmt::terminal_color::cyan))
        );
        ::qmlRegisterModule(this->name_.c_str(), this->version_.major, this->version_.minor);
      }
    }

    template <___concept___(std::derived_from<::QObject>) T ___sfinae_requirement___((std::is_base_of_v<::QObject, T>))>
    module& component(std::optional<std::string_view> name = std::nullopt) {
      auto const component_name = module::demangle_class_name<T>(name);
      if constexpr(implicit_verbosity == verbosity::verbose)
        fmt::println(
          "rolly::qt::qml: \tregistering type {} (qobject)",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::green) | fmt::emphasis::bold)
        );
      ::qmlRegisterType<T>(this->name_.c_str(), this->version_.major, this->version_.minor, component_name.c_str());
      return *this;
    }

    template <___concept___(concepts::qobject) T ___sfinae_requirement___(is_qobject_v<T>)>
    module& singleton(T* instance, std::optional<std::string_view> name = std::nullopt) {
      auto const component_name = module::demangle_class_name<T>(name);
      if constexpr(implicit_verbosity == verbosity::verbose)
        fmt::println(
          "rolly::qt::qml: \tregistering singleton type {} (instance)",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::magenta) | fmt::emphasis::bold)
        );
      ::qmlRegisterSingletonInstance(
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str(),
        instance
      );
      return *this;
    }

    template <___concept___(concepts::qobject) T ___sfinae_requirement___(is_qobject_v<T>)>
    module& singleton(std::optional<std::string_view> name = std::nullopt) {
      auto const component_name = module::demangle_class_name<T>(name);
      if constexpr(implicit_verbosity == verbosity::verbose)
        fmt::println(
          "rolly::qt::qml: \tregistering singleton type {}",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::bright_magenta) | fmt::emphasis::bold)
        );
      ::qmlRegisterSingletonType<
        T>(this->name_.c_str(), this->version_.major, this->version_.minor, component_name.c_str(), T::create);
      return *this;
    }

    module& file(std::string_view url, std::optional<std::string_view> name = std::nullopt) {
      auto const component_name = module::demangle_file_url(url, name);
      if constexpr(implicit_verbosity == verbosity::verbose)
        fmt::println(
          "rolly::qt::qml: \tregistering type {} from {}",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::yellow) | fmt::emphasis::bold),
          fmt::styled(url, fmt::emphasis::faint)
        );
      ::qmlRegisterType(
        ::QUrl(url.data()),  // NOLINT(*-suspicious-stringview-data-usage)
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str()
      );
      return *this;
    }

    template <___concept___(concepts::qgadget) T ___sfinae_requirement___(is_qgadget_v<T>)>
    module& uncreatable(
      std::optional<std::string_view> name = std::nullopt,
      std::optional<std::string_view> reason = std::nullopt
    ) {
      auto const component_name = module::demangle_class_name<T>(name);
      auto const reason_string = [&]() -> std::string {
        if(not reason.has_value())
          return fmt::format("Class \'{}\' is uncreatable", component_name);
        return std::string(reason.value());
      }();
      if constexpr(implicit_verbosity == verbosity::verbose)
        fmt::println(
          "rolly::qt::qml: \tregistering uncreatable type {}, reason: {}",
          fmt::styled(component_name, fmt::fg(fmt::terminal_color::bright_cyan) | fmt::emphasis::bold),
          fmt::styled(reason_string, fmt::fg(fmt::terminal_color::cyan))
        );
      ::qmlRegisterUncreatableType<T>(
        this->name_.c_str(),
        this->version_.major,
        this->version_.minor,
        component_name.c_str(),
        ::QString::fromStdString(reason_string)
      );
      return *this;
    }

    module& push(registrable& r) {
      r.register_in(*this);
      return *this;
    }

    module& push_all(std::list<registrable>&& r) {
      contracts::precondition(r.size() > 0);
      for(auto& i : r)
        i.register_in(*this);
      return *this;
    }

   private:
    template <___concept___(concepts::qgadget) T ___sfinae_requirement___(is_qgadget_v<T>)>
    [[nodiscard]] static std::string demangle_class_name(std::optional<std::string_view> name) {
      if(name)
        return std::string(*name);
      auto const meta_name = detail::strip(T::staticMetaObject.className(), detail::strip_kind::namespace_);
      return detail::strip(meta_name, detail::strip_kind::hungarian_prefix);
    }

    [[nodiscard]] static std::string demangle_file_url(std::string_view url, std::optional<std::string_view> name) {
      if(name)
        return std::string(*name);
      try {
        auto const res = detail::strip(url, detail::strip_kind::prefix_and_extension);
        return res;
      } catch(std::exception const& err) {
        if constexpr(implicit_verbosity == verbosity::verbose)
          fmt::println(
            stderr,
            "rolly::qt::qml: error during qml registration: {}",
            fmt::styled(err.what(), fmt::fg(fmt::terminal_color::red) | fmt::emphasis::bold)
          );
        return "Unknown";
      }
    }

    std::string name_;
    version_type version_;
  };
}  // namespace rolly::qt::qml
#endif  // defined(ROLLY_QT_GUI)
