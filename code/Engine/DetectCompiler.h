#pragma once

// Architecture
#define SE_ARCH_32BIT              0
#define SE_ARCH_64BIT              0

// Debug
#define SE_DEBUG                   0

// Compiler
#define SE_COMPILER_MSVC           0
#define SE_COMPILER_CLANG          0
#define SE_COMPILER_CLANG_ANALYZER 0
#define SE_COMPILER_GNUC           0
#define SE_COMPILER_INTEL          0
#define SE_COMPILER_MINGW          0

// CPU
#define SE_CPU_ARM                 0
#define SE_CPU_JIT                 0
#define SE_CPU_MIPS                0
#define SE_CPU_PPC                 0
#define SE_CPU_RISCV               0
#define SE_CPU_X86                 0

// Endianess
#define SE_CPU_ENDIAN_BIG          0
#define SE_CPU_ENDIAN_LITTLE       0

//=============================================================================
// Architecture
//=============================================================================
#if defined(__x86_64__)    || \
	defined(_M_X64)        || \
	defined(__aarch64__)   || \
	defined(__64BIT__)     || \
	defined(__mips64)      || \
	defined(__powerpc64__) || \
	defined(__ppc64__)     || \
	defined(__LP64__)
#	undef  SE_ARCH_64BIT
#	define SE_ARCH_64BIT 64
#else
#	undef  SE_ARCH_32BIT
#	define SE_ARCH_32BIT 32
#endif

//=============================================================================
// DEBUG MODE
//=============================================================================
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(NDEBUG)
#	undef  SE_DEBUG
#	define SE_DEBUG 1
#endif

//=============================================================================
// Compiler
//=============================================================================
#if defined(__clang__)
#	undef  SE_COMPILER_CLANG
#	define SE_COMPILER_CLANG __clang_version__
#	if defined(__clang_analyzer__)
#		undef  SE_COMPILER_CLANG_ANALYZER
#		define SE_COMPILER_CLANG_ANALYZER 1
#	endif // defined(__clang_analyzer__)
#elif defined(__GNUC__) // Check after Clang, as Clang defines this too
#	undef  SE_COMPILER_GNUC
#	define SE_COMPILER_GNUC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__INTEL_COMPILER)
#	undef  SE_COMPILER_INTEL
#	define SE_COMPILER_INTEL __INTEL_COMPILER
#elif defined(__MINGW32__) || defined(__MINGW64__)
#	undef  SE_COMPILER_MINGW
#	define SE_COMPILER_MINGW 1
#elif defined(_MSC_VER)
#	undef  SE_COMPILER_MSVC
#	define SE_COMPILER_MSVC _MSC_VER
#else
#	error "Unknown compiler."
#endif

//=============================================================================
// CPU
//=============================================================================

#if defined(__arm__)     \
 || defined(__aarch64__) \
 || defined(_M_ARM)
#	undef  SE_CPU_ARM
#	define SE_CPU_ARM 1
#	define SE_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__)     \
 ||   defined(__mips_isa_rev) \
 ||   defined(__mips64)
#	undef  SE_CPU_MIPS
#	define SE_CPU_MIPS 1
#	define SE_CACHE_LINE_SIZE 64
#elif defined(_M_PPC)        \
 ||   defined(__powerpc__)   \
 ||   defined(__powerpc64__)
#	undef  SE_CPU_PPC
#	define SE_CPU_PPC 1
#	define SE_CACHE_LINE_SIZE 128
#elif defined(__riscv)   \
 ||   defined(__riscv__) \
 ||   defined(RISCVEL)
#	undef  SE_CPU_RISCV
#	define SE_CPU_RISCV 1
#	define SE_CACHE_LINE_SIZE 64
#elif defined(_M_IX86)    \
 ||   defined(_M_X64)     \
 ||   defined(__i386__)   \
 ||   defined(__x86_64__)
#	undef  SE_CPU_X86
#	define SE_CPU_X86 1
#	define SE_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
#	undef  SE_CPU_JIT
#	define SE_CPU_JIT 1
#	define SE_CACHE_LINE_SIZE 64
#endif

//=============================================================================
// Endianess
//=============================================================================
#if SE_CPU_PPC
// __BIG_ENDIAN__ is gcc predefined macro
#	if defined(__BIG_ENDIAN__)
#		undef  SE_CPU_ENDIAN_BIG
#		define SE_CPU_ENDIAN_BIG 1
#	else
#		undef  SE_CPU_ENDIAN_LITTLE
#		define SE_CPU_ENDIAN_LITTLE 1
#	endif
#else
#	undef  SE_CPU_ENDIAN_LITTLE
#	define SE_CPU_ENDIAN_LITTLE 1
#endif