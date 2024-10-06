#pragma once

#include <fmt/format.h>
#include "version_definitions.h"
#include "semver.h"

namespace rolly // NOLINT(*-concat-nested-namespaces)
{
  /**
   * @brief Library meta information.
   */
  namespace meta
  {
    /**
     * @brief Immutable constant expression structure, holding project meta-information.
     * @details See @ref library_metadata global variable for current <b>floppy</b> project meta-information.
     * @ingroup meta
     * @see version
     */
    class [[maybe_unused]] project_meta
    {
     public:
      /**
       * @brief Creates project meta-information object at compile time.
       * @param version Project version.
       * @param name Project name.
       * @param domain Project domain.
       * @param organization Project organization.
       */
      constexpr project_meta(
        struct version version,
        std::string_view name,
        std::string_view domain,
        std::string_view organization
      ) noexcept
        : version_(version)
        , name_(name)
        , domain_(domain)
        , organization_(organization)
      {}

      /**
       * @brief Version of the project.
       */
      [[nodiscard]] constexpr struct version version() const noexcept { return this->version_; }

      /**
       * @brief Name of the project.
       */
      [[nodiscard]] constexpr std::string_view name() const noexcept { return this->name_; }

      /**
       * @brief Project domain.
       */
      [[nodiscard]] constexpr std::string_view domain() const noexcept { return this->domain_; }

      /**
       * @brief Project organization.
       */
      [[nodiscard]] constexpr std::string_view organization() const noexcept { return this->organization_; }

     private:
      struct version version_;
      std::string_view name_;
      std::string_view domain_;
      std::string_view organization_;
    };

    /**
     * @ingroup meta
     * @brief Constant expression, containing library meta-information.
     * @details This constant expression is evaluated at compile time.
     * Example content of this variable:
     * @code
     * version:
     *   major: 2
     *   minor: 0
     *   patch: 0
     * name: 'rolly'
     * domain: 'com'
     * organization: 'Radar MMS'
     * @endcode
     * @see project_meta, version
     */
    [[maybe_unused]] constexpr inline auto library_metadata = project_meta(
      version(
        ROLLY_VERSION_MAJOR,
        ROLLY_VERSION_MINOR,
        ROLLY_VERSION_PATCH
      ),
      "rolly",
      "com",
      "Radar MMS"
    );
  } // namespace meta
} // namespace rolly

static_assert(rolly::meta::library_metadata.version().major == ROLLY_VERSION_MAJOR);
static_assert(rolly::meta::library_metadata.version().minor == ROLLY_VERSION_MINOR);
static_assert(rolly::meta::library_metadata.version().patch == ROLLY_VERSION_PATCH);
static_assert(rolly::meta::library_metadata.version() == rolly::version("2.0.0"));
static_assert(alignof(rolly::version) == 2);
static_assert(sizeof(rolly::version) == 12);
static_assert(alignof(rolly::prerelease) == 1);
static_assert(sizeof(rolly::prerelease) == 1);