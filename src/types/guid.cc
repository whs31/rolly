#include <rolly/types/guid.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <rolly/global/platform_definitions.h>

#if defined(ROLLY_OS_LINUX)
#  include <uuid/uuid.h>
#elif defined(ROLLY_OS_ANDROID)
#  include "oslayer/android/guid.h"
#elif defined(ROLLY_OS_WINDOWS)
#  include <objbase.h>
#endif

namespace rolly {
  inline namespace types {
    guid::guid(std::array<std::byte, 16> const& bytes) {
      std::memcpy(this->bytes_.data(), bytes.data(), 16);
    }

    std::string guid::to_string() const {
      return fmt::format(
        "{:02x}{:02x}{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:"
        "02x}{:02x}",
        this->bytes_[0],
        this->bytes_[1],
        this->bytes_[2],
        this->bytes_[3],
        this->bytes_[4],
        this->bytes_[5],
        this->bytes_[6],
        this->bytes_[7],
        this->bytes_[8],
        this->bytes_[9],
        this->bytes_[10],
        this->bytes_[11],
        this->bytes_[12],
        this->bytes_[13],
        this->bytes_[14],
        this->bytes_[15]
      );
    }

    std::ostream& operator<<(std::ostream& os, guid const& guid) {
      auto flags = os.flags();
      os << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(guid.bytes()[0])
         << std::setw(2) << static_cast<int>(guid.bytes()[1]) << std::setw(2)
         << static_cast<int>(guid.bytes()[2]) << std::setw(2) << static_cast<int>(guid.bytes()[3])
         << "-" << std::setw(2) << static_cast<int>(guid.bytes()[4]) << std::setw(2)
         << static_cast<int>(guid.bytes()[5]) << "-" << std::setw(2)
         << static_cast<int>(guid.bytes()[6]) << std::setw(2) << static_cast<int>(guid.bytes()[7])
         << "-" << std::setw(2) << static_cast<int>(guid.bytes()[8]) << std::setw(2)
         << static_cast<int>(guid.bytes()[9]) << "-" << std::setw(2)
         << static_cast<int>(guid.bytes()[10]) << std::setw(2) << static_cast<int>(guid.bytes()[11])
         << std::setw(2) << static_cast<int>(guid.bytes()[12]) << std::setw(2)
         << static_cast<int>(guid.bytes()[13]) << std::setw(2) << static_cast<int>(guid.bytes()[14])
         << std::setw(2) << static_cast<int>(guid.bytes()[15]);
      os.flags(flags);
      return os;
    }

    guid guid::random() noexcept {
#if defined(ROLLY_OS_LINUX)
      static_assert(
        std::is_same_v<unsigned char[16], uuid_t>,
        "rolly::guid: uuid_t is not unsigned char[16]"
      );  // NOLINT(*-avoid-c-arrays)

      auto data = std::array<u8, 16>();
      ::uuid_generate(data.data());
      return guid(data);
#elif defined(ROLLY_OS_ANDROID)
#  warning "rolly::guid::random() is currently not implemented for Android"
      return guid({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
#elif defined(ROLLY_OS_WINDOWS)
      auto new_id = GUID();
      ::CoCreateGuid(&new_id);
      auto data = std::array<u8, 16> {
        static_cast<u8>(new_id.Data1 >> 24 & 0xFF),
        static_cast<u8>(new_id.Data1 >> 16 & 0xFF),
        static_cast<u8>(new_id.Data1 >> 8 & 0xFF),
        static_cast<u8>(new_id.Data1 & 0xFF),
        static_cast<u8>(new_id.Data2 >> 8 & 0xFF),
        static_cast<u8>(new_id.Data2 & 0xFF),
        static_cast<u8>(new_id.Data3 >> 8 & 0xFF),
        static_cast<u8>(new_id.Data3 & 0xFF),
        static_cast<u8>(new_id.Data4[0]),
        static_cast<u8>(new_id.Data4[1]),
        static_cast<u8>(new_id.Data4[2]),
        static_cast<u8>(new_id.Data4[3]),
        static_cast<u8>(new_id.Data4[4]),
        static_cast<u8>(new_id.Data4[5]),
        static_cast<u8>(new_id.Data4[6]),
        static_cast<u8>(new_id.Data4[7])
      };
      return guid(data);
#endif  // ROLLY_OS_LINUX
    }

    u64 guid::to_u64() const noexcept {
      return detail::hash<rolly::u64, rolly::u64> {}(bytes()[0], bytes()[1]);
    }
  }  // namespace types
}  // namespace rolly
