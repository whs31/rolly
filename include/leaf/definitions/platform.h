#pragma once

// os detection
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) ||            \
  defined(__WINDOWS__)
#define leaf_os_windows
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define leaf_os_linux
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
#define leaf_os_darwin
#elif defined(__ANDROID__)
#define leaf_os_android
#elif defined(__APPLE__) && defined(__MACH__) &&                                                   \
  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#define leaf_os_ios
#elif defined(__CYGWIN__)
#define leaf_os_cygwin
#elif defined(__FreeBSD__)
#define leaf_os_freebsd
#elif defined(__DragonFly__)
#define leaf_os_dragonfly
#elif defined(__NETBSD__) || defined(__NetBSD__)
#define leaf_os_netbsd
#elif defined(__OpenBSD__)
#define leaf_os_openbsd
#else
#define leaf_os_unknown
#warning "-- [leaf] Operating system is not detected."
#endif

// arch detection
#if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) ||                \
  defined(__i686__) || defined(__i386) || defined(_M_IX86) || defined(_X86_) ||                    \
  defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__)
#define leaf_arch_x86_32
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) ||        \
  defined(_M_X64)
#define leaf_arch_x86_64
#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#define leaf_arch_alpha
#elif defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) ||         \
  defined(_M_ARM) || defined(__arm__) || defined(__arm64) || defined(__thumb__) ||                 \
  defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) ||                           \
  defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||               \
  defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||              \
  defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) ||           \
  defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
#define leaf_arch_arm
#elif defined(__bfin__) || defined(__BFIN__) || defined(bfin) || defined(BFIN)
#define leaf_arch_bfin
#elif defined(__convex__)
#define leaf_arch_convex
#elif defined(__e2k__)
#define leaf_arch_e2k
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) ||               \
  defined(_M_IA64) || defined(__itanium__)
#define leaf_arch_ia64
#elif defined(__loongarch__)
#define leaf_arch_loongarch
#elif defined(__m68k__) || defined(M68000)
#define leaf_arch_m68k
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__)
#define leaf_arch_mips
#elif defined(__hppa__) || defined(__hppa) || defined(__HPPA__)
#define leaf_arch_hppa
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||                      \
  defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) ||            \
  defined(__PPC64__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) ||           \
  defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc)
#define leaf_arch_powerpc
#elif defined(__CUDA_ARCH__)
#define leaf_arch_cuda
#elif defined(pyr)
#define leaf_arch_pyramid
#elif defined(__riscv)
#define leaf_arch_riscv
#elif defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) ||         \
  defined(_ARCH_PWR2)
#define leaf_arch_rs6000
#elif defined(__sparc__) || defined(__sparc)
#define leaf_arch_sparc
#elif defined(__sh__)
#define leaf_arch_superh
#elif defined(__370__) || defined(__THW_370__)
#define leaf_arch_sys370
#elif defined(__s390__) || defined(__s390x__)
#define leaf_arch_sys390
#elif defined(__SYSC_ZARCH__)
#define leaf_arch_sysz
#else
#define leaf_arch_unknown
#warning "-- [leaf] Architecture is not detected."
#endif

// compiler detection

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#define leaf_compiler_borland
#elif defined(__clang__)
#define leaf_compiler_clang
#elif defined(__COMO__)
#define leaf_compiler_como
#elif defined(__DECC) || defined(__DECCXX)
#define leaf_compiler_compaq
#elif defined(__DCC__)
#define leaf_compiler_diab
#elif defined(__DMC__)
#define leaf_compiler_dmc
#elif defined(__SYSC__)
#define leaf_compiler_dignus
#elif defined(__EDG__)
#define leaf_compiler_edg
#elif defined(__PATHCC__)
#define leaf_compiler_pathscale
#elif defined(__GNUC__)
#define leaf_compiler_gnu
#elif defined(__ghs) || defined(__ghs__)
#define leaf_compiler_ghs
#elif defined(__HP_aCC)
#define leaf_compiler_hpa
#elif defined(__IAR_SYSTEMS_ICC__)
#define leaf_compiler_iar
#elif defined(__IBMCPP__) || defined(__xlC__) || defined(__xlc__)
#define leaf_compiler_ibm
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#define leaf_compiler_intel
#elif defined(__KCC)
#define leaf_compiler_kai
#elif defined(__llvm__)
#define leaf_compiler_llvm
#elif defined(__HIGHC__)
#define leaf_compiler_highc
#elif defined(__MWERKS__) || defined(__CWCC__)
#define leaf_compiler_metrowerks
#elif defined(_MRI)
#define leaf_compiler_mri
#elif defined(__MRC__) || defined(MPW_C) || defined(MPW_CPLUS)
#define leaf_compiler_mpw
#elif defined(__NVCC__)
#define leaf_compiler_nvidia
#elif defined(_PACC_VER)
#define leaf_compiler_pacc
#elif defined(__PGI)
#define leaf_compiler_pgi
#elif defined(__sgi) || defined(sgi)
#define leaf_compiler_sgi
#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#define leaf_compiler_sun
#elif defined(__TenDRA__)
#define leaf_compiler_tendra
#elif defined(_MSC_VER)
#define leaf_compiler_msvc
#elif defined(__WATCOMC__)
#define leaf_compiler_watcom
#else
#define leaf_compiler_unknown
#warning "-- [leaf] Compiler is not detected."
#endif

// endianness detection

#if (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)) ||                                    \
  (defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)) || defined(__ARMEB__) ||                      \
  defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) ||       \
  defined(__MIPSEB__)
#define leaf_endian_big
#elif (defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)) ||                                  \
  (defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)) || defined(__ARMEL__) ||                      \
  defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(__loongarch__) || defined(_MIPSEL) ||  \
  defined(__MIPSEL) || defined(__MIPSEL__) || defined(__riscv) || defined(__e2k__)
#define leaf_endian_little
#else
#define leaf_endian_unknown
#endif

// C++ standard detection

#if !defined(__cplusplus)
#define leaf_language_c
#else
#if __cplusplus == 1
#define leaf_language_cxxpre98
#elif __cplusplus <= 199811L
#define leaf_language_cxx98
#elif __cplusplus <= 200911L
#define leaf_language_cxx03
#elif __cplusplus <= 201104L
#define leaf_language_cxx11
#elif __cplusplus <= 201404L
#define leaf_language_cxx14
#elif __cplusplus <= 201704L
#define leaf_language_cxx17
#elif __cplusplus <= 202004L
#define leaf_language_cxx20
#elif __cplusplus > 202004L
#define leaf_language_cxx23
#else
#define leaf_language_unknown
#endif
#endif