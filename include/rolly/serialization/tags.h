#pragma once

namespace rolly
{
  /**
   * @brief Functions and types related to serialization and deserialization.
   */
  namespace serialization {} // namespace serialization
} // namespace rolly

namespace rolly::serialization
{
  /**
   * @brief Serialization tags.
   */
  namespace format
  {
    struct json {};
    struct bson {};
    struct xml {};
    struct cbor {};
    struct flexbuffer {};
    struct msgpack {};
    struct toml {};
    struct yaml {};
    struct generic {};
  } // namespace tag
} // namespace rolly::serialization