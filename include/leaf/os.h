#pragma once

#include <limits>
#include <leaf/global/types.h>
#include <leaf/global/definitions.h>
#include <leaf/definitions/platform.h>

/**
 * \brief Пространство имен для функций и классов, связанных с платформами, ОС и архитектурой
 */
namespace leaf::os {
  enum class operating_system
  {
    windows,
    gnu_linux,
    darwin,
    android,
    cygwin,
    freebsd,
    dragonfly,
    netbsd,
    openbsd,
    unknown
  };

  enum class arch
  {
    x86_32,
    x86_64,
    alpha,
    arm,
    bfin,
    convex,
    e2k,
    ia64,
    loongarch,
    m68k,
    mips,
    hppa,
    powerpc,
    cuda,
    pyramid,
    riscv,
    rs6000,
    sparc,
    superh,
    s370,
    s390,
    sysz,
    unknown
  };

  enum class compiler
  {
    borland,
    clang,
    comeau,
    compaq,
    diab,
    digitalmars,
    dignussystem,
    edg,
    pathscale,
    gcc,
    greenhills,
    hpa,
    iar,
    ibm,
    intel,
    kai,
    llvm,
    metaware,
    codewarrior,
    microtec,
    mpw,
    nvcc,
    palm,
    portlandgroup,
    mipspro,
    oracle,
    tendra,
    msvc,
    watcom,
    unknown
  };

  enum class endianness
  {
    little,
    big,
    unknown
  };

  enum class language
  {
    c,
    cxxpre98,
    cxx98,
    cxx03,
    cxx11,
    cxx14,
    cxx17,
    cxx20,
    cxx23,
    unknown
  };

  [[nodiscard]] consteval auto system() noexcept -> operating_system
  {
#if defined(leaf_os_windows)
    return operating_system::windows;
#elif defined(leaf_os_linux)
    return operating_system::gnu_linux;
#elif defined(leaf_os_darwin)
    return operating_system::darwin;
#elif defined(leaf_os_android)
    return operating_system::android;
#elif defined(leaf_os_ios)
    return operating_system::ios;
#elif defined(leaf_os_cygwin)
    return operating_system::cygwin;
#elif defined(leaf_os_freebsd)
    return operating_system::freebsd;
#elif defined(leaf_os_dragonfly)
    return operating_system::dragonfly;
#elif defined(leaf_os_netbsd)
    return operating_system::netbsd;
#elif defined(leaf_os_openbsd)
    return operating_system::openbsd;
#else
    return operating_system::unknown;
#endif
  }

  [[nodiscard]] consteval auto architecture() noexcept -> arch
  {
#if defined(leaf_arch_x86_32)
    return arch::x86_32;
#elif defined(leaf_arch_x86_64)
    return arch::x86_64;
#elif defined(leaf_arch_alpha)
    return arch::alpha;
#elif defined(leaf_arch_arm)
    return arch::arm;
#elif defined(leaf_arch_bfin)
    return arch::bfin;
#elif defined(leaf_arch_convex)
    return arch::convex;
#elif defined(leaf_arch_e2k)
    return arch::e2k;
#elif defined(leaf_arch_ia64)
    return arch::ia64;
#elif defined(leaf_arch_loongarch)
    return arch::loongarch;
#elif defined(leaf_arch_m68k)
    return arch::m68k;
#elif defined(leaf_arch_mips)
    return arch::mips;
#elif defined(leaf_arch_hppa)
    return arch::hppa;
#elif defined(leaf_arch_powerpc)
    return arch::powerpc;
#elif defined(leaf_arch_cuda)
    return arch::cuda;
#elif defined(leaf_arch_pyramid)
    return arch::pyramid;
#elif defined(leaf_arch_riscv)
    return arch::riscv;
#elif defined(leaf_arch_rs6000)
    return arch::rs6000;
#elif defined(leaf_arch_sparc)
    return arch::sparc;
#elif defined(leaf_arch_superh)
    return arch::superh;
#elif defined(leaf_arch_s370)
    return arch::s370;
#elif defined(leaf_arch_s390)
    return arch::s390;
#elif defined(leaf_arch_sysz)
    return arch::sysz;
#else
    return arch::unknown;
#endif
  }

  [[nodiscard]] consteval auto compiler() noexcept -> compiler
  {
#if defined(leaf_compiler_borland)
    return compiler::borland;
#elif defined(leaf_compiler_clang)
    return compiler::clang;
#elif defined(leaf_compiler_como)
    return compiler::comeau;
#elif defined(leaf_compiler_compaq)
    return compiler::compaq;
#elif defined(leaf_compiler_diab)
    return compiler::diab;
#elif defined(leaf_compiler_dmc)
    return compiler::digitalmars;
#elif defined(leaf_compiler_dignus)
    return compiler::dignussystem;
#elif defined(leaf_compiler_edg)
    return compiler::edg;
#elif defined(leaf_compiler_pathscale)
    return compiler::pathscale;
#elif defined(leaf_compiler_gnu)
    return compiler::gcc;
#elif defined(leaf_compiler_ghs)
    return compiler::greenhills;
#elif defined(leaf_compiler_hpa)
    return compiler::hpa;
#elif defined(leaf_compiler_iar)
    return compiler::iar;
#elif defined(leaf_compiler_ibm)
    return compiler::ibm;
#elif defined(leaf_compiler_intel)
    return compiler::intel;
#elif defined(leaf_compiler_kai)
    return compiler::kai;
#elif defined(leaf_compiler_llvm)
    return compiler::llvm;
#elif defined(leaf_compiler_highc)
    return compiler::metaware;
#elif defined(leaf_compiler_metrowerks)
    return compiler::codewarrior;
#elif defined(leaf_compiler_mri)
    return compiler::microtec;
#elif defined(leaf_compiler_mpw)
    return compiler::mpw;
#elif defined(leaf_compiler_nvidia)
    return compiler::nvcc;
#elif defined(leaf_compiler_pacc)
    return compiler::palm;
#elif defined(leaf_compiler_pgi)
    return compiler::portlandgroup;
#elif defined(leaf_compiler_mipspro)
    return compiler::mipspro;
#elif defined(leaf_compiler_sun)
    return compiler::oracle;
#elif defined(leaf_compiler_tendra)
    return compiler::tendra;
#elif defined(leaf_compiler_msvc)
    return compiler::msvc;
#elif defined(leaf_compiler_watcom)
    return compiler::watcom;
#else
    return compiler::unknown;
#endif
  }

  [[nodiscard]] consteval auto endianness() noexcept -> endianness
  {
#if defined(leaf_endian_little)
    return endianness::little;
#elif defined(leaf_endian_big)
    return endianness::big;
#else
    return endianness::unknown;
#endif
  }

  [[nodiscard]] consteval auto language() noexcept -> language
  {
#if defined(leaf_language_c)
    return language::c;
#elif defined(leaf_language_cxxpre98)
    return language::cxxpre98;
#elif defined(leaf_language_cxx98)
    return language::cxx98;
#elif defined(leaf_language_cxx03)
    return language::cxx03;
#elif defined(leaf_language_cxx11)
    return language::cxx11;
#elif defined(leaf_language_cxx14)
    return language::cxx14;
#elif defined(leaf_language_cxx17)
    return language::cxx17;
#elif defined(leaf_language_cxx20)
    return language::cxx20;
#elif defined(leaf_language_cxx23)
    return language::cxx23;
#else
    return language::unknown;
#endif
  }

  [[nodiscard]] consteval auto path_separator() noexcept -> char
  {
    switch (system()) {
      case operating_system::windows: return '\\';
      default: return '/';
    }
  }

  template <typename T> auto swap_endian(T u) -> T
  {
    using std::numeric_limits;
    static_assert(numeric_limits<unsigned char>::digits == 8, "unsigned char is not 8 bits");

    union
    {
      T u;
      u8 c[sizeof(T)];
    } source {}, dest {};
    source.u = u;
    for (size_t k = 0; k < sizeof(T); k++)
      dest.c[k] = source.c[sizeof(T) - k - 1];
    return dest.u;
  }
} // namespace leaf::os