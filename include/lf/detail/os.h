#pragma once

#include <limits>
#include <lf/detail/types.h>
#include <lf/detail/export.h>
#include <lf/detail/platform.h>

/// \brief Namespace providing access to operating system, compiler and platform information.
namespace lf::os
{
  /// \brief Operating systems supported by Leaf.
  enum class operating_system
  {
    windows,       ///< Microsoft Windows
    gnu_linux,     ///< GNU/Linux
    darwin,        ///< Apple macOS
    android,       ///< Android
    cygwin,        ///< Cygwin
    freebsd,       ///< FreeBSD
    dragonfly,     ///< DragonFly BSD
    netbsd,        ///< NetBSD
    openbsd,       ///< OpenBSD
    unknown        ///< Unknown OS - unable to determine
  };

  /// \brief Architectures supported by Leaf.
  enum class arch
  {
    x86_32,       ///< 32-bit x86
    x86_64,       ///< 64-bit x86
    alpha,        ///< Alpha
    arm,          ///< ARM (any)
    bfin,         ///< Blackfin
    convex,       ///< Convex
    e2k,          ///< Elbrus 2k
    ia64,         ///< 64-bit Itanium
    loongarch,    ///< LoongArch
    m68k,         ///< 68k
    mips,         ///< Mips
    hppa,         ///< HP-PA
    powerpc,      ///< PowerPC
    cuda,         ///< CUDA
    pyramid,      ///< Pyramid
    riscv,        ///< RISC-V
    rs6000,       ///< RS6000
    sparc,        ///< Sparc
    superh,       ///< SuperH
    s370,         ///< 370
    s390,         ///< 390
    sysz,         ///< SystemZ
    unknown       ///< Unknown architecture - unable to determine
  };

  /// \brief Compilers supported by Leaf.
  enum class compiler
  {
    borland,       ///< Borland C++ Builder
    clang,         ///< Clang
    comeau,        ///< Comeau C++
    compaq,        ///< Compaq C++
    diab,          ///< Diab C++
    digitalmars,   ///< Digital Mars
    dignussystem,  ///< Dignus Systems C++
    edg,           ///< EDG C++
    pathscale,     ///< PathScale C++
    gcc,           ///< GNU GCC
    greenhills,    ///< Greenhills C++
    hpa,           ///< Hewlett-Packard C++
    iar,           ///< IAR C++
    ibm,           ///< IBM C++
    intel,         ///< Intel C++
    kai,           ///< Kai C++
    llvm,          ///< LLVM C++
    metaware,      ///< Metaware C++
    codewarrior,   ///< CodeWarrior C++
    microtec,      ///< Microtec C++
    mpw,           ///< MPW C++
    nvcc,          ///< NVIDIA C++
    palm,          ///< Palm C++
    portlandgroup, ///< Portland Group C++
    mipspro,       ///< MIPSpro C++
    oracle,        ///< Oracle C++ (Sun)
    tendra,        ///< Tendra C++
    msvc,          ///< Microsoft Visual C++
    watcom,        ///< Watcom C++
    unknown        ///< Unknown compiler - unable to determine
  };

  /// \brief Endiannesses enumeration.
  enum class endianness
  {
    little,     ///< Little-endian
    big,        ///< Big-endian
    unknown     ///< Unknown endianness - unable to determine
  };

  /// \brief Returns constant-evaluated current operating system.
  [[nodiscard]] consteval auto system() noexcept -> operating_system
  {
    #if defined(LF_OS_WINDOWS)
    return operating_system::windows;
    #elif defined(LF_OS_LINUX)
    return operating_system::gnu_linux;
    #elif defined(LF_OS_DARWIN)
    return operating_system::darwin;
    #elif defined(LF_OS_ANDROID)
    return operating_system::android;
    #elif defined(LF_OS_IOS)
    return operating_system::ios;
    #elif defined(LF_OS_CYGWIN)
    return operating_system::cygwin;
    #elif defined(LF_OS_FREEBSD)
    return operating_system::freebsd;
    #elif defined(LF_OS_DRAGONFLY)
    return operating_system::dragonfly;
    #elif defined(LF_OS_NETBSD)
    return operating_system::netbsd;
    #elif defined(LF_OS_OPENBSD)
    return operating_system::openbsd;
    #else
    return operating_system::unknown;
    #endif
  }

  /// \brief Returns constant-evaluated current architecture.
  [[nodiscard]] consteval auto architecture() noexcept -> arch
  {
    #if defined(LF_ARCH_X86_32)
    return arch::x86_32;
    #elif defined(LF_ARCH_X86_64)
    return arch::x86_64;
    #elif defined(LF_ARCH_ALPHA)
    return arch::alpha;
    #elif defined(LF_ARCH_ARM)
    return arch::arm;
    #elif defined(LF_ARCH_BFIN)
    return arch::bfin;
    #elif defined(LF_ARCH_CONVEX)
    return arch::convex;
    #elif defined(LF_ARCH_E2K)
    return arch::e2k;
    #elif defined(LF_ARCH_IA64)
    return arch::ia64;
    #elif defined(LF_ARCH_LOONGARCH)
    return arch::loongarch;
    #elif defined(LF_ARCH_M68K)
    return arch::m68k;
    #elif defined(LF_ARCH_MIPS)
    return arch::mips;
    #elif defined(LF_ARCH_HPPA)
    return arch::hppa;
    #elif defined(LF_ARCH_POWERPC)
    return arch::powerpc;
    #elif defined(LF_ARCH_CUDA)
    return arch::cuda;
    #elif defined(LF_ARCH_PYRAMID)
    return arch::pyramid;
    #elif defined(LF_ARCH_RISCV)
    return arch::riscv;
    #elif defined(LF_ARCH_RS6000)
    return arch::rs6000;
    #elif defined(LF_ARCH_SPARC)
    return arch::sparc;
    #elif defined(LF_ARCH_SUPERH)
    return arch::superh;
    #elif defined(LF_ARCH_S370)
    return arch::s370;
    #elif defined(LF_ARCH_S390)
    return arch::s390;
    #elif defined(LF_ARCH_SYSZ)
    return arch::sysz;
    #else
    return arch::unknown;
    #endif
  }

  /// \brief Returns constant-evaluated current compiler.
  [[nodiscard]] consteval auto compiler() noexcept -> compiler
  {
    #if defined(LF_COMPILER_BORLAND)
    return compiler::borland;
    #elif defined(LF_COMPILER_CLANG)
    return compiler::clang;
    #elif defined(LF_COMPILER_COMO)
    return compiler::comeau;
    #elif defined(LF_COMPILER_COMPAQ)
    return compiler::compaq;
    #elif defined(LF_COMPILER_DIAB)
    return compiler::diab;
    #elif defined(LF_COMPILER_DMC)
    return compiler::digitalmars;
    #elif defined(LF_COMPILER_DIGNUS)
    return compiler::dignussystem;
    #elif defined(LF_COMPILER_EDG)
    return compiler::edg;
    #elif defined(LF_COMPILER_PATHSCALE)
    return compiler::pathscale;
    #elif defined(LF_COMPILER_GNU)
    return compiler::gcc;
    #elif defined(LF_COMPILER_GHS)
    return compiler::greenhills;
    #elif defined(LF_COMPILER_HPA)
    return compiler::hpa;
    #elif defined(LF_COMPILER_IAR)
    return compiler::iar;
    #elif defined(LF_COMPILER_IBM)
    return compiler::ibm;
    #elif defined(LF_COMPILER_INTEL)
    return compiler::intel;
    #elif defined(LF_COMPILER_KAI)
    return compiler::kai;
    #elif defined(LF_COMPILER_LLVM)
    return compiler::llvm;
    #elif defined(LF_COMPILER_HIGHC)
    return compiler::metaware;
    #elif defined(LF_COMPILER_METROWERKS)
    return compiler::codewarrior;
    #elif defined(LF_COMPILER_MRI)
    return compiler::microtec;
    #elif defined(LF_COMPILER_MPW)
    return compiler::mpw;
    #elif defined(LF_COMPILER_NVIDIA)
    return compiler::nvcc;
    #elif defined(LF_COMPILER_PACC)
    return compiler::palm;
    #elif defined(LF_COMPILER_PGI)
    return compiler::portlandgroup;
    #elif defined(LF_COMPILER_MIPSPRO)
    return compiler::mipspro;
    #elif defined(LF_COMPILER_SUN)
    return compiler::oracle;
    #elif defined(LF_COMPILER_TENDRA)
    return compiler::tendra;
    #elif defined(LF_COMPILER_MSVC)
    return compiler::msvc;
    #elif defined(LF_COMPILER_WATCOM)
    return compiler::watcom;
    #else
    return compiler::unknown;
    #endif
  }

  /// \brief Returns constant-evaluated current endianness.
  [[nodiscard]] consteval auto endianness() noexcept -> endianness
  {
    #if defined(LF_ENDIAN_LITTLE)
    return endianness::little;
    #elif defined(LF_ENDIAN_BIG)
    return endianness::big;
    #else
    return endianness::unknown;
    #endif
  }

  /// \brief Returns constant-evaluated current language.
  [[nodiscard]] consteval auto language() noexcept -> i16
  {
    #if defined(LF_LANGUAGE_C)
    return std::numeric_limits<i16>::min();
    #elif defined(LF_LANGUAGE_CXXPRE98)
    return -3;
    #elif defined(LF_LANGUAGE_CXX98)
    return -2;
    #elif defined(LF_LANGUAGE_CXX03)
    return 3;
    #elif defined(LF_LANGUAGE_CXX11)
    return 11;
    #elif defined(LF_LANGUAGE_CXX14)
    return 14;
    #elif defined(LF_LANGUAGE_CXX17)
    return 17;
    #elif defined(LF_LANGUAGE_CXX20)
    return 20;
    #elif defined(LF_LANGUAGE_CXX23)
    return 23;
    #else
    return -1;
    #endif
  }

  /**
   * \brief Returns constant-evaluated file system path separator character on the current platforms.
   * \details Returns '/' for POSIX and '\\' for Windows.
   * \return File system path separator character.
   */
  [[nodiscard]] consteval auto path_separator() noexcept -> char
  {
    switch (system()) {
      case operating_system::windows: return '\\';
      default: return '/';
    }
  }

  /**
   * \brief Swaps endianness of the given value.
   * \tparam T Value type.
   * \param u Value.
   * \return Value with swapped endianness.
   */
  template <typename T>
  auto swap_endian(T u) -> T
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
} // namespace lf::os