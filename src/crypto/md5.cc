#include <rll/crypto/md5.h>

namespace {
  using namespace rll;

  inline u32 f1(u32 const b, u32 const c, u32 const d) {
    return d ^ (b & (c ^ d));  // original: f = (b & c) | ((~b) & d);
  }

  inline u32 f2(u32 const b, u32 const c, u32 const d) {
    return c ^ (d & (b ^ c));  // original: f = (b & d) | (c & (~d));
  }

  inline u32 f3(u32 const b, u32 const c, u32 const d) { return b ^ c ^ d; }

  inline u32 f4(u32 const b, u32 const c, u32 const d) { return c ^ (b | ~d); }

  inline u32 rotate(u32 const a, u32 const c) { return (a << c) | (a >> (32 - c)); }

#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
  inline u32 swap(u32 const x) {
#  if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(x);
#  endif
#  ifdef MSC_VER
    return _byteswap_ulong(x);
#  endif
    return (x >> 24) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | (x << 24);
  }
#endif
}  // namespace

namespace rll::crypto {
  struct md5::md5_private {
    mutable u64 num_bytes;
    mutable std::size_t buffer_size;
    mutable std::array<u8, block_size> buffer;
    mutable std::array<u32, digest_size / sizeof(u32)> state;

    md5_private() { this->reset(); }

    void process_block(void const* block) const {
      auto a = this->state[0];
      auto b = this->state[1];
      auto c = this->state[2];
      auto d = this->state[3];
      auto const* words = static_cast<u32 const*>(block);

#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
#  define LITTLEENDIAN(x) swap(x)
#else
#  define LITTLEENDIAN(x) (x)
#endif

      // first round
      auto const word0 = LITTLEENDIAN(words[0]);
      a = rotate(a + f1(b, c, d) + word0 + 0xd76aa478, 7) + b;
      auto const word1 = LITTLEENDIAN(words[1]);
      d = rotate(d + f1(a, b, c) + word1 + 0xe8c7b756, 12) + a;
      auto const word2 = LITTLEENDIAN(words[2]);
      c = rotate(c + f1(d, a, b) + word2 + 0x242070db, 17) + d;
      auto const word3 = LITTLEENDIAN(words[3]);
      b = rotate(b + f1(c, d, a) + word3 + 0xc1bdceee, 22) + c;

      auto const word4 = LITTLEENDIAN(words[4]);
      a = rotate(a + f1(b, c, d) + word4 + 0xf57c0faf, 7) + b;
      auto const word5 = LITTLEENDIAN(words[5]);
      d = rotate(d + f1(a, b, c) + word5 + 0x4787c62a, 12) + a;
      auto const word6 = LITTLEENDIAN(words[6]);
      c = rotate(c + f1(d, a, b) + word6 + 0xa8304613, 17) + d;
      auto const word7 = LITTLEENDIAN(words[7]);
      b = rotate(b + f1(c, d, a) + word7 + 0xfd469501, 22) + c;

      auto const word8 = LITTLEENDIAN(words[8]);
      a = rotate(a + f1(b, c, d) + word8 + 0x698098d8, 7) + b;
      auto const word9 = LITTLEENDIAN(words[9]);
      d = rotate(d + f1(a, b, c) + word9 + 0x8b44f7af, 12) + a;
      auto const word10 = LITTLEENDIAN(words[10]);
      c = rotate(c + f1(d, a, b) + word10 + 0xffff5bb1, 17) + d;
      auto const word11 = LITTLEENDIAN(words[11]);
      b = rotate(b + f1(c, d, a) + word11 + 0x895cd7be, 22) + c;

      auto const word12 = LITTLEENDIAN(words[12]);
      a = rotate(a + f1(b, c, d) + word12 + 0x6b901122, 7) + b;
      auto const word13 = LITTLEENDIAN(words[13]);
      d = rotate(d + f1(a, b, c) + word13 + 0xfd987193, 12) + a;
      auto const word14 = LITTLEENDIAN(words[14]);
      c = rotate(c + f1(d, a, b) + word14 + 0xa679438e, 17) + d;
      auto const word15 = LITTLEENDIAN(words[15]);
      b = rotate(b + f1(c, d, a) + word15 + 0x49b40821, 22) + c;

      a = rotate(a + f2(b, c, d) + word1 + 0xf61e2562, 5) + b;
      d = rotate(d + f2(a, b, c) + word6 + 0xc040b340, 9) + a;
      c = rotate(c + f2(d, a, b) + word11 + 0x265e5a51, 14) + d;
      b = rotate(b + f2(c, d, a) + word0 + 0xe9b6c7aa, 20) + c;

      a = rotate(a + f2(b, c, d) + word5 + 0xd62f105d, 5) + b;
      d = rotate(d + f2(a, b, c) + word10 + 0x02441453, 9) + a;
      c = rotate(c + f2(d, a, b) + word15 + 0xd8a1e681, 14) + d;
      b = rotate(b + f2(c, d, a) + word4 + 0xe7d3fbc8, 20) + c;

      a = rotate(a + f2(b, c, d) + word9 + 0x21e1cde6, 5) + b;
      d = rotate(d + f2(a, b, c) + word14 + 0xc33707d6, 9) + a;
      c = rotate(c + f2(d, a, b) + word3 + 0xf4d50d87, 14) + d;
      b = rotate(b + f2(c, d, a) + word8 + 0x455a14ed, 20) + c;

      a = rotate(a + f2(b, c, d) + word13 + 0xa9e3e905, 5) + b;
      d = rotate(d + f2(a, b, c) + word2 + 0xfcefa3f8, 9) + a;
      c = rotate(c + f2(d, a, b) + word7 + 0x676f02d9, 14) + d;
      b = rotate(b + f2(c, d, a) + word12 + 0x8d2a4c8a, 20) + c;

      a = rotate(a + f3(b, c, d) + word5 + 0xfffa3942, 4) + b;
      d = rotate(d + f3(a, b, c) + word8 + 0x8771f681, 11) + a;
      c = rotate(c + f3(d, a, b) + word11 + 0x6d9d6122, 16) + d;
      b = rotate(b + f3(c, d, a) + word14 + 0xfde5380c, 23) + c;

      a = rotate(a + f3(b, c, d) + word1 + 0xa4beea44, 4) + b;
      d = rotate(d + f3(a, b, c) + word4 + 0x4bdecfa9, 11) + a;
      c = rotate(c + f3(d, a, b) + word7 + 0xf6bb4b60, 16) + d;
      b = rotate(b + f3(c, d, a) + word10 + 0xbebfbc70, 23) + c;

      a = rotate(a + f3(b, c, d) + word13 + 0x289b7ec6, 4) + b;
      d = rotate(d + f3(a, b, c) + word0 + 0xeaa127fa, 11) + a;
      c = rotate(c + f3(d, a, b) + word3 + 0xd4ef3085, 16) + d;
      b = rotate(b + f3(c, d, a) + word6 + 0x04881d05, 23) + c;

      a = rotate(a + f3(b, c, d) + word9 + 0xd9d4d039, 4) + b;
      d = rotate(d + f3(a, b, c) + word12 + 0xe6db99e5, 11) + a;
      c = rotate(c + f3(d, a, b) + word15 + 0x1fa27cf8, 16) + d;
      b = rotate(b + f3(c, d, a) + word2 + 0xc4ac5665, 23) + c;

      a = rotate(a + f4(b, c, d) + word0 + 0xf4292244, 6) + b;
      d = rotate(d + f4(a, b, c) + word7 + 0x432aff97, 10) + a;
      c = rotate(c + f4(d, a, b) + word14 + 0xab9423a7, 15) + d;
      b = rotate(b + f4(c, d, a) + word5 + 0xfc93a039, 21) + c;

      a = rotate(a + f4(b, c, d) + word12 + 0x655b59c3, 6) + b;
      d = rotate(d + f4(a, b, c) + word3 + 0x8f0ccc92, 10) + a;
      c = rotate(c + f4(d, a, b) + word10 + 0xffeff47d, 15) + d;
      b = rotate(b + f4(c, d, a) + word1 + 0x85845dd1, 21) + c;

      a = rotate(a + f4(b, c, d) + word8 + 0x6fa87e4f, 6) + b;
      d = rotate(d + f4(a, b, c) + word15 + 0xfe2ce6e0, 10) + a;
      c = rotate(c + f4(d, a, b) + word6 + 0xa3014314, 15) + d;
      b = rotate(b + f4(c, d, a) + word13 + 0x4e0811a1, 21) + c;

      a = rotate(a + f4(b, c, d) + word4 + 0xf7537e82, 6) + b;
      d = rotate(d + f4(a, b, c) + word11 + 0xbd3af235, 10) + a;
      c = rotate(c + f4(d, a, b) + word2 + 0x2ad7d2bb, 15) + d;
      b = rotate(b + f4(c, d, a) + word9 + 0xeb86d391, 21) + c;

      this->state[0] += a;
      this->state[1] += b;
      this->state[2] += c;
      this->state[3] += d;
    }

    void process_all() const {
      auto padded_len = this->buffer_size * 8;
      padded_len++;
      if(auto const lower_11_bits = padded_len & 511; lower_11_bits <= 448)
        padded_len += 448 - lower_11_bits;
      else
        padded_len += 512 + 448 - lower_11_bits;
      padded_len /= 8;

      auto extra = std::array<u8, block_size>();
      if(this->buffer_size < block_size)
        this->buffer[this->buffer_size] = 128;
      else
        extra[0] = 128;

      auto i = std::size_t();
      for(i = this->buffer_size + 1; i < block_size; i++)
        this->buffer[i] = 0;
      for(; i < padded_len; i++)
        extra[i - md5::block_size] = 0;

      auto msg_bits = 8 * this->num_bytes + this->buffer_size;
      auto* add_length = static_cast<u8*>(nullptr);
      if(padded_len < md5::block_size)
        add_length = this->buffer.data() + padded_len;
      else
        add_length = extra.data() + padded_len - md5::block_size;

      // must be little endian
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;
      msg_bits >>= 8;
      *add_length++ = msg_bits & 0xFF;

      this->process_block(this->buffer.data());
      if(padded_len > block_size)
        this->process_block(extra.data());
    }

    void reset() {
      this->num_bytes = 0;
      this->buffer_size = 0;
      this->state = {
        0x67452301,
        0xefcdab89,
        0x98badcfe,
        0x10325476,
      };
    }
  };

  md5::md5()
    : impl(std::make_unique<md5_private>()) {}

  md5::~md5() = default;

  basic_hasher& md5::append(std::string const& str) {
    return this->append(str.c_str(), str.size());
  }

  basic_hasher& md5::append(std::string_view const str) {
    return this->append(str.data(), str.size());
  }

  basic_hasher& md5::append(void const* str, std::size_t len) {
    auto const* current = static_cast<u8 const*>(str);

    if(impl->buffer_size > 0) {
      while(len > 0 and impl->buffer_size < md5::block_size) {
        impl->buffer[impl->buffer_size++] = *current++;
        len--;
      }
    }

    if(impl->buffer_size == md5::block_size) {
      impl->process_block(impl->buffer.data());
      impl->num_bytes += md5::block_size;
      impl->buffer_size = 0;
    }

    if(len == 0)
      return *this;

    while(len >= md5::block_size) {
      impl->process_block(current);
      current += md5::block_size;
      impl->num_bytes += md5::block_size;
      len -= md5::block_size;
    }

    while(len > 0) {
      impl->buffer[impl->buffer_size++] = *current++;
      len--;
    }

    return *this;
  }

  void md5::reset() { impl->reset(); }

  std::string md5::hash_string() const {
    auto const hash = this->hash();
    auto res = std::string();
    res.reserve(2 * md5::digest_size);
    for(auto i = 0; i < md5::digest_size; i++) {
      res.push_back("0123456789abcdef"[(hash[i] >> 4) & 0xF]);
      res.push_back("0123456789abcdef"[hash[i] & 0xF]);
    }
    return res;
  }

  md5::digest_type md5::hash() const {  // save old hash if buffer is partially filled
    auto const old = impl->state;
    impl->process_all();

    auto current = md5::digest_type();
    auto* current_ptr = current.data();
    for(auto i = 0; i < md5::digest_size / sizeof(u32); i++) {
      *current_ptr++ = impl->state[i] & 0xFF;
      *current_ptr++ = (impl->state[i] >> 8) & 0xFF;
      *current_ptr++ = (impl->state[i] >> 16) & 0xFF;
      *current_ptr++ = (impl->state[i] >> 24) & 0xFF;
      impl->state[i] = old[i];
    }

    return current;
  }

  uuid md5::hash_uuid() const { return uuid(this->hash()); }
}  // namespace rll::crypto
