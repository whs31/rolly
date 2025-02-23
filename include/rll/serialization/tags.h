#pragma once

namespace rll {
  /**
   * @brief Functions and types related to serialization and deserialization.
   */
  namespace serialization {}  // namespace serialization
}  // namespace rll

namespace rll::serialization {
  /**
   * @brief Serialization tags.
   */
  namespace format {
    struct json {};

    struct bson {};

    struct xml {};

    struct cbor {};

    struct flexbuffer {};

    struct msgpack {};

    struct toml {};

    struct yaml {};

    struct generic {};
  }  // namespace format
}  // namespace rll::serialization
