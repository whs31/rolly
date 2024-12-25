#pragma once

#if defined(DOXYGEN)
/**
 * @ingroup platform
 * @brief Flag defined if operating system is Windows.
 */
#  define ROLLY_OS_WINDOWS

/**
 * @ingroup platform
 * @brief Flag defined if operating system is Linux.
 */
#  define ROLLY_OS_LINUX

/**
 * @ingroup platform
 * @brief Flag defined if operating system is macOS.
 */
#  define ROLLY_OS_DARWIN

/**
 * @ingroup platform
 * @brief Flag defined if operating system is Android.
 */
#  define ROLLY_OS_ANDROID

/**
 * @ingroup platform
 * @brief Flag defined if operating system is Apple iOS.
 */
#  define ROLLY_OS_IOS

/**
 * @ingroup platform
 * @brief Flag defined if operating system is Cygwin.
 */
#  define ROLLY_OS_CYGWIN

/**
 * @ingroup platform
 * @brief Flag defined if operating system is FreeBSD.
 */
#  define ROLLY_OS_FREEBSD

/**
 * @ingroup platform
 * @brief Flag defined if operating system is DragonFly BSD.
 */
#  define ROLLY_OS_DRAGONFLY

/**
 * @ingroup platform
 * @brief Flag defined if operating system is NetBSD.
 */
#  define ROLLY_OS_NETBSD

/**
 * @ingroup platform
 * @brief Flag defined if operating system is OpenBSD.
 */
#  define ROLLYOPPY_OS_OPENBSD

/**
 * @ingroup platform
 * @brief Flag defined if operating system is unknown.
 */
#  define ROLLY_OS_UNKNOWN

/**
 * @ingroup platform
 * @brief Flag defined if architecture is x86_32.
 */
#  define ROLLY_ARCH_X86_32

/**
 * @ingroup platform
 * @brief Flag defined if architecture is x86_64.
 */
#  define ROLLY_ARCH_X86_64

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Alpha.
 * @see https://en.wikipedia.org/wiki/DEC_Alpha
 */
#  define ROLLY_ARCH_ALPHA

/**
 * @ingroup platform
 * @brief Flag defined if architecture is ARM.
 */
#  define ROLLY_ARCH_ARM

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Blackfin.
 */
#  define ROLLY_ARCH_BFIN

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Convex.
 */
#  define ROLLY_ARCH_CONVEX

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Elbrus 2K.
 * @sa https://en.wikipedia.org/wiki/Elbrus_2000
 */
#  define ROLLY_ARCH_E2K

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Intel IA64.
 * @sa https://ru.wikipedia.org/wiki/Intel_Architecture
 */
#  define ROLLY_ARCH_IA64

/**
 * @ingroup platform
 * @brief Flag defined if architecture is LoongArch.
 * @sa https://en.wikipedia.org/wiki/Loongson
 */
#  define ROLLY_ARCH_LOONGARCH

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Motorola M68K.
 * @sa https://en.wikipedia.org/wiki/Motorola_680x0
 */
#  define ROLLY_ARCH_M68K

/**
 * @ingroup platform
 * @brief Flag defined if architecture is MIPS.
 * @sa https://en.wikipedia.org/wiki/MIPS_architecture
 */
#  define ROLLY_ARCH_MIPS

/**
 * @ingroup platform
 * @brief Flag defined if architecture is PA-RISC (HPPA).
 * @sa https://en.wikipedia.org/wiki/PA-RISC
 */
#  define ROLLY_ARCH_HPPA

/**
 * @ingroup platform
 * @brief Flag defined if architecture is PowerPC.
 * @sa https://en.wikipedia.org/wiki/PowerPC
 */
#  define ROLLY_ARCH_POWERPC

/**
 * @ingroup platform
 * @brief Flag defined if architecture is NVIDIA CUDA.
 * @note This does not imply that <code>__CUDA_ARCH__</code> or <code>__CUDACC__</code> is defined.
 * @sa https://en.wikipedia.org/wiki/CUDA
 */
#  define ROLLY_ARCH_CUDA

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Pyramid.
 * @sa https://en.wikipedia.org/wiki/Pyramid_Technology
 */
#  define ROLLY_ARCH_PYRAMID

/**
 * @ingroup platform
 * @brief Flag defined if architecture is RISC-V.
 * @sa https://en.wikipedia.org/wiki/RISC-V
 */
#  define ROLLY_ARCH_RISCV

/**
 * @ingroup platform
 * @brief Flag defined if architecture is IBM RS/6000.
 * @sa https://en.wikipedia.org/wiki/IBM_RS/6000
 */
#  define ROLLY_ARCH_RS6000

/**
 * @ingroup platform
 * @brief Flag defined if architecture is SPARC.
 * @sa https://en.wikipedia.org/wiki/SPARC
 */
#  define ROLLY_ARCH_SPARC

/**
 * @ingroup platform
 * @brief Flag defined if architecture is Hitachi SuperH.
 * @sa https://en.wikipedia.org/wiki/SuperH
 */
#  define ROLLY_ARCH_SUPERH

/**
 * @ingroup platform
 * @brief Flag defined if architecture is unknown.
 */
#  define ROLLY_ARCH_UNKNOWN

/**
 * @ingroup platform
 * @brief Flag defined if compiler is Clang LLVM.
 */
#  define ROLLY_COMPILER_CLANG

/**
 * @ingroup platform
 * @brief Flag defined if compiler is GNU GCC.
 */
#  define ROLLY_COMPILER_GNU

/**
 * @ingroup platform
 * @brief Flag defined if compiler is Intel C++ Compiler.
 */
#  define ROLLY_COMPILER_INTEL

/**
 * @ingroup platform
 * @brief Flag defined if compiler is Nvidia NVCC.
 */
#  define ROLLY_COMPILER_NVIDIA

/**
 * @ingroup platform
 * @brief Flag defined if compiler is Microsoft Visual C++ (MSVC).
 */
#  define ROLLY_COMPILER_MSVC

/**
 * @ingroup platform
 * @brief Flag defined if compiler is unknown.
 */
#  define ROLLY_COMPILER_UNKNOWN

/**
 * @ingroup platform
 * @brief Flag defined if architecture is big endian.
 * @bug Can often be undefined on some platforms. Consider using <code>std::endian::native</code> to
 * determine endianness.
 * @sa https://en.wikipedia.org/wiki/Endianness
 */
#  define ROLLY_ENDIAN_BIG

/**
 * @ingroup platform
 * @brief Flag defined if architecture is little endian.
 * @bug Can often be undefined on some platforms. Consider using <code>std::endian::native</code> to
 * determine endianness.
 * @sa https://en.wikipedia.org/wiki/Endianness
 */
#  define ROLLY_ENDIAN_LITTLE

/**
 * @ingroup platform
 * @brief Flag defined if architecture is unknown.
 */
#  define ROLLY_ENDIAN_UNKNOWN
#else
#  if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) \
    || defined(__WINDOWS__) || defined(DOXYGEN)
#    define ROLLY_OS_WINDOWS
#  elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__) \
    || defined(DOXYGEN)
#    define ROLLY_OS_LINUX
#  elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__)) \
    || defined(DOXYGEN)
#    define ROLLY_OS_DARWIN
#  elif defined(__ANDROID__) || defined(DOXYGEN)
#    define ROLLY_OS_ANDROID
#  elif defined(__APPLE__) && defined(__MACH__)                  \
      && defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
    || defined(DOXYGEN)
#    define ROLLY_OS_IOS
#  elif defined(__CYGWIN__) || defined(DOXYGEN)
#    define ROLLY_OS_CYGWIN
#  elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(DOXYGEN)
#    define ROLLY_OS_FREEBSD
#  elif defined(__DragonFly__) || defined(DOXYGEN)
#    define ROLLY_OS_DRAGONFLY
#  elif defined(__NETBSD__) || defined(__NetBSD__) || defined(DOXYGEN)
#    define ROLLY_OS_NETBSD
#  elif defined(__OpenBSD__) || defined(DOXYGEN)
#    define ROLLY_OS_OPENBSD
#  else
#    define ROLLY_OS_UNKNOWN
#    warning "-- [rolly] Operating system is not detected."
#  endif  // OS
#  if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) \
    || defined(__i686__) || defined(__i386) || defined(_M_IX86) || defined(_X86_)  \
    || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__)            \
    || defined(DOXYGEN)
#    define ROLLY_ARCH_X86_32
#  elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) \
    || defined(_M_X64) || defined(DOXYGEN)
#    define ROLLY_ARCH_X86_64
#  elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#    define ROLLY_ARCH_ALPHA
#  elif defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB)  \
    || defined(_M_ARM) || defined(__arm__) || defined(__arm64) || defined(__thumb__)       \
    || defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)                 \
    || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__)     \
    || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__)    \
    || defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) \
    || defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)   \
    || defined(DOXYGEN)
#    define ROLLY_ARCH_ARM
#  elif defined(__bfin__) || defined(__BFIN__) || defined(bfin) || defined(BFIN) \
    || defined(DOXYGEN)
#    define ROLLY_ARCH_BFIN
#  elif defined(__convex__) || defined(DOXYGEN)
#    define ROLLY_ARCH_CONVEX
#  elif defined(__e2k__) || defined(DOXYGEN)
#    define ROLLY_ARCH_E2K
#  elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) \
    || defined(_M_IA64) || defined(__itanium__) || defined(DOXYGEN)
#    define ROLLY_ARCH_IA64
#  elif defined(__loongarch__) || defined(DOXYGEN)
#    define ROLLY_ARCH_LOONGARCH
#  elif defined(__m68k__) || defined(M68000) || defined(DOXYGEN)
#    define ROLLY_ARCH_M68K
#  elif defined(__mips__) || defined(__mips) || defined(__MIPS__) \
    || defined(DOXYGEN)
#    define ROLLY_ARCH_MIPS
#  elif defined(__hppa__) || defined(__hppa) || defined(__HPPA__) \
    || defined(DOXYGEN)
#    define ROLLY_ARCH_HPPA
#  elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__)                  \
    || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__)     \
    || defined(__PPC64__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)    \
    || defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc) \
    || defined(DOXYGEN)
#    define ROLLY_ARCH_POWERPC
#  elif defined(__CUDA_ARCH__) || defined(DOXYGEN)
#    define ROLLY_ARCH_CUDA
#  elif defined(pyr) || defined(DOXYGEN)
#    define ROLLY_ARCH_PYRAMID
#  elif defined(__riscv) || defined(DOXYGEN)
#    define ROLLY_ARCH_RISCV
#  elif defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) \
    || defined(_ARCH_PWR2) || defined(DOXYGEN)
#    define ROLLY_ARCH_RS6000
#  elif defined(__sparc__) || defined(__sparc) || defined(DOXYGEN)
#    define ROLLY_ARCH_SPARC
#  elif defined(__sh__) || defined(DOXYGEN_GENERATING_OUTPUT)
#    define ROLLY_ARCH_SUPERH
#  else
#    define ROLLY_ARCH_UNKNOWN
#    warning "-- [rolly] Architecture is not detected."
#  endif  // ARCH
#  if defined(__clang__) || defined(DOXYGEN)
#    define ROLLY_COMPILER_CLANG
#  elif defined(__GNUC__) || defined(DOXYGEN)
#    define ROLLY_COMPILER_GNU
#  elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC) \
    || defined(DOXYGEN)
#    define ROLLY_COMPILER_INTEL
#  elif defined(__NVCC__) || defined(DOXYGEN)
#    define ROLLY_COMPILER_NVIDIA
#  elif defined(_MSC_VER) || defined(DOXYGEN)
#    define ROLLY_COMPILER_MSVC
#  else
#    define ROLLY_COMPILER_UNKNOWN
#    warning "-- [rolly] Compiler is not detected."
#  endif  // COMPILER
#  if (defined(__BIG_ENDIAN__) && ! defined(__LITTLE_ENDIAN__))                                \
    || (defined(_BIG_ENDIAN) && ! defined(_LITTLE_ENDIAN)) || defined(__ARMEB__)               \
    || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) \
    || defined(__MIPSEB__) || defined(DOXYGEN)
#    define ROLLY_ENDIAN_BIG
#  elif (defined(__LITTLE_ENDIAN__) && ! defined(__BIG_ENDIAN__))                       \
    || (defined(_LITTLE_ENDIAN) && ! defined(_BIG_ENDIAN)) || defined(__ARMEL__)        \
    || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(__loongarch__)         \
    || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || defined(__riscv) \
    || defined(__e2k__) || defined(DOXYGEN)
#    define ROLLY_ENDIAN_LITTLE
#  else
#    define ROLLY_ENDIAN_UNKNOWN
#  endif  // ENDIAN
#endif    // DOXYGEN
