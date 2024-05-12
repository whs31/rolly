#pragma once

// os detection
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) ||            \
  defined(__WINDOWS__)
#define LF_OS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define LF_OS_LINUX
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
#define LF_OS_DARWIN
#elif defined(__ANDROID__)
#define LF_OS_ANDROID
#elif defined(__APPLE__) && defined(__MACH__) &&                                                   \
  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#define LF_OS_IOS
#elif defined(__CYGWIN__)
#define LF_OS_CYGWIN
#elif defined(__FreeBSD__)
#define LF_OS_FREEBSD
#elif defined(__DragonFly__)
#define LF_OS_DRAGONFLY
#elif defined(__NETBSD__) || defined(__NetBSD__)
#define LF_OS_NETBSD
#elif defined(__OpenBSD__)
#define LF_OS_OPENBSD
#else
#define LF_OS_UNKNOWN
#warning "-- [leaf] Operating system is not detected."
#endif

// arch detection
#if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) ||                \
  defined(__i686__) || defined(__i386) || defined(_M_IX86) || defined(_X86_) ||                    \
  defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__)
#define LF_ARCH_X86_32
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) ||        \
  defined(_M_X64)
#define LF_ARCH_X86_64
#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#define LF_ARCH_ALPHA
#elif defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) ||         \
  defined(_M_ARM) || defined(__arm__) || defined(__arm64) || defined(__thumb__) ||                 \
  defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) ||                           \
  defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||               \
  defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||              \
  defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) ||           \
  defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
#define LF_ARCH_ARM
#elif defined(__bfin__) || defined(__BFIN__) || defined(bfin) || defined(BFIN)
#define LF_ARCH_BFIN
#elif defined(__convex__)
#define LF_ARCH_CONVEX
#elif defined(__e2k__)
#define LF_ARCH_E2K
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) ||               \
  defined(_M_IA64) || defined(__itanium__)
#define LF_ARCH_IA64
#elif defined(__loongarch__)
#define LF_ARCH_LOONGARCH
#elif defined(__m68k__) || defined(M68000)
#define LF_ARCH_M68K
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__)
#define LF_ARCH_MIPS
#elif defined(__hppa__) || defined(__hppa) || defined(__HPPA__)
#define LF_ARCH_HPPA
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||                      \
  defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) ||            \
  defined(__PPC64__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) ||           \
  defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc)
#define LF_ARCH_POWERPC
#elif defined(__CUDA_ARCH__)
#define LF_ARCH_CUDA
#elif defined(pyr)
#define LF_ARCH_PYRAMID
#elif defined(__riscv)
#define LF_ARCH_RISCV
#elif defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) ||         \
  defined(_ARCH_PWR2)
#define LF_ARCH_RS6000
#elif defined(__sparc__) || defined(__sparc)
#define LF_ARCH_SPARC
#elif defined(__sh__)
#define LF_ARCH_SUPERH
#elif defined(__370__) || defined(__THW_370__)
#define LF_ARCH_SYS370
#elif defined(__s390__) || defined(__s390x__)
#define LF_ARCH_SYS390
#elif defined(__SYSC_ZARCH__)
#define LF_ARCH_SYSZ
#else
#define LF_ARCH_UNKNOWN
#warning "-- [leaf] Architecture is not detected."
#endif

// compiler detection

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#define LF_COMPILER_BORLAND
#elif defined(__clang__)
#define LF_COMPILER_CLANG
#elif defined(__COMO__)
#define LF_COMPILER_COMO
#elif defined(__DECC) || defined(__DECCXX)
#define LF_COMPILER_COMPAQ
#elif defined(__DCC__)
#define LF_COMPILER_DIAB
#elif defined(__DMC__)
#define LF_COMPILER_DMC
#elif defined(__SYSC__)
#define LF_COMPILER_DIGNUS
#elif defined(__EDG__)
#define LF_COMPILER_EDG
#elif defined(__PATHCC__)
#define LF_COMPILER_PATHSCALE
#elif defined(__GNUC__)
#define LF_COMPILER_GNU
#elif defined(__ghs) || defined(__ghs__)
#define LF_COMPILER_GHS
#elif defined(__HP_aCC)
#define LF_COMPILER_HPA
#elif defined(__IAR_SYSTEMS_ICC__)
#define LF_COMPILER_IAR
#elif defined(__IBMCPP__) || defined(__xlC__) || defined(__xlc__)
#define LF_COMPILER_IBM
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#define LF_COMPILER_INTEL
#elif defined(__KCC)
#define LF_COMPILER_KAI
#elif defined(__llvm__)
#define LF_COMPILER_LLVM
#elif defined(__HIGHC__)
#define LF_COMPILER_HIGHC
#elif defined(__MWERKS__) || defined(__CWCC__)
#define LF_COMPILER_METROWERKS
#elif defined(_MRI)
#define LF_COMPILER_MRI
#elif defined(__MRC__) || defined(MPW_C) || defined(MPW_CPLUS)
#define LF_COMPILER_MPW
#elif defined(__NVCC__)
#define LF_COMPILER_NVIDIA
#elif defined(_PACC_VER)
#define LF_COMPILER_PACC
#elif defined(__PGI)
#define LF_COMPILER_PGI
#elif defined(__sgi) || defined(sgi)
#define LF_COMPILER_SGI
#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#define LF_COMPILER_SUN
#elif defined(__TenDRA__)
#define LF_COMPILER_TENDRA
#elif defined(_MSC_VER)
#define LF_COMPILER_MSVC
#elif defined(__WATCOMC__)
#define LF_COMPILER_WATCOM
#else
#define LF_COMPILER_UNKNOWN
#warning "-- [leaf] Compiler is not detected."
#endif

// endianness detection

#if (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)) ||                                    \
  (defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)) || defined(__ARMEB__) ||                      \
  defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) ||       \
  defined(__MIPSEB__)
#define LF_ENDIAN_BIG
#elif (defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)) ||                                  \
  (defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)) || defined(__ARMEL__) ||                      \
  defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(__loongarch__) || defined(_MIPSEL) ||  \
  defined(__MIPSEL) || defined(__MIPSEL__) || defined(__riscv) || defined(__e2k__)
#define LF_ENDIAN_LITTLE
#else
#define LF_ENDIAN_UNKNOWN
#endif

// C++ standard detection

#if !defined(__cplusplus)
#define LF_LANGUAGE_C
#else
#if __cplusplus == 1
#define LF_LANGUAGE_CXXPRE98
#elif __cplusplus <= 199811L
#define LF_LANGUAGE_CXX98
#elif __cplusplus <= 200911L
#define LF_LANGUAGE_CXX03
#elif __cplusplus <= 201104L
#define LF_LANGUAGE_CXX11
#elif __cplusplus <= 201404L
#define LF_LANGUAGE_CXX14
#elif __cplusplus <= 201704L
#define LF_LANGUAGE_CXX17
#elif __cplusplus <= 202004L
#define LF_LANGUAGE_CXX20
#elif __cplusplus > 202004L
#define LF_LANGUAGE_CXX23
#else
#define LF_LANGUAGE_UNKNOWN
#endif
#endif