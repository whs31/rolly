#pragma once

#include <array>
#include <rll/stdint.h>
#include <rll/uuid.h>
#include <rll/traits/pimpl.h>
#include <rll/crypto/basic_hasher.h>

namespace rll::crypto {
  class RLL_API md5 : public basic_hasher {
   public:
    using digest_type = std::array<u8, 16>;

    inline static constexpr std::size_t block_size = 64;
    inline static constexpr std::size_t digest_size = 16;

    md5();
    md5(md5 const&) = delete;
    md5(md5&&) noexcept = delete;
    md5& operator=(md5 const&) = delete;
    md5& operator=(md5&&) noexcept = delete;
    ~md5() override;

    basic_hasher& append(std::string const& str) override;
    basic_hasher& append(std::string_view str) override;
    basic_hasher& append(void const* str, std::size_t len) override;

    void reset() override;

    [[nodiscard]] std::string hash_string() const override;
    [[nodiscard]] digest_type hash() const;
    [[nodiscard]] uuid hash_uuid() const;

   private:
    DECLARE_PRIVATE_AS(md5_private)
  };
}  // namespace rll::crypto
