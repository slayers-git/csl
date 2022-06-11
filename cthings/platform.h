/* platform.h - determine platform the program is being compiled for */
/* Sergey Lafin, 2022 */

#ifndef __CSL_PLATFORM__
#define __CSL_PLATFORM__

/*********************/
/* Operating Systems */
/*********************/

/* Linux */
#if defined (__linux__) || defined (__linux) || defined (linux)
#   define CSL_PLATFORM_LINUX
#   define CSL_PLATFORM_UNIX
#   define CSL_PLATFORM_POSIX
/* Windows 32 and 64 bit */
#elif defined (_WIN32) || defined (__MINGW32__)
#   define CSL_PLATFORM_WINDOWS
/* Cygwin is POSIX-compliant */
#   if defined (__CYGWIN__)
#       define CSL_PLATFORM_POSIX
#   endif
#elif defined (__APPLE__)
#   include <AvailabilityMacros.h>
#   include <TargetConditionals.h>
#   define CSL_PLATFORM_POSIX 
#   define CSL_PLATFORM_UNIX
/*  iOS */
#   if defined (TARGET_OS_IOS) || defined (TARGET_OS_IPHONE)
#       define CSL_PLATFORM_IOS
/*  AppleTV */
#   elif defined (TARGET_OS_TV)
#       define CSL_PLATFORM_APPLETV
#   else
/*  MacOS */
#       define CSL_PLATFORM_MACOS
#   endif
/* FreeBSD */
#elif defined (__FreeBSD__) || defined (__DragonFly__)
#   define CSL_PLATFORM_FREEBSD
#   define CSL_PLATFORM_POSIX 
#   define CSL_PLATFORM_UNIX
#else 
#   define CSL_PLATFORM_UNKNOWN
#endif

static const char *csl_platform_os (void) {
#ifdef CSL_PLATFORM_LINUX 
    return "Linux";
#elif defined (CSL_PLATFORM_WINDOWS)
    return "Windows";
#elif defined (CSL_PLATFORM_MACOS)
    return "MacOS";
#elif defined (CSL_PLATFORM_IOS)
    return "iOS";
#elif defined (CSL_PLATFORM_APPLETV)
    return "Apple TV";
#elif defined (CSL_PLATFORM_FREEBSD)
    return "FreeBSD";
#elif defined (CSL_PLATFORM_UNKNOWN)
    return "Unknown OS";
#endif
}

/*****************/
/* Architectures */
/*****************/

/* With the help of: https://sourceforge.net/p/predef/wiki/Architectures */

#if defined (__x86_64__) || defined (__x86_64) || defined (__amd64) || defined (__amd64__) || defined (_M_X64) || defined (_M_AMD64)
#   define CSL_PLATFORM_AMD64
#   define CSL_PLATFORM_x86_64
#   define CSL_PLATFORM_x86_64
#elif defined (__i386__) || defined (__i386) || defined (i386) || defined (_M_IX86) || defined (_X86_)
#   define CSL_PLATFORM_X86 
#elif defined (__arm__) || defined (_M_ARM) || defined (_M_ARMT)
#   ifdef __aarch64__
#       define CSL_PLATFORM_ARM64
#   endif
#   define CSL_PLATFORM_ARM
#elif defined (__mips__) || defined (__mips) || defined (mips)
#   define CSL_PLATFORM_MIPS
#elif defined (__PPC__) || defined (__powerpc__) || defined (__powerpc) || defined (__powerpc__) || defined (_M_PPC)
#   ifdef __ppc64__
#       define CSL_PLATFORM_PPC64
#   endif
#   define CSL_PLATFORM_PPC
#else
#   define CSL_PLATFORM_UNKNOWNARCH
#endif

static const char *csl_platform_arch (void) {
#if defined (CSL_PLATFORM_AMD64)
    return "x86_64";
#elif defined (CSL_PLATFORM_X86)
    return "x86";
#elif defined (CSL_PLATFORM_ARM)
#   ifdef CSL_PLATFORM_ARM64
        return "AARCH64";
#   else
        return "ARM";
#   endif
#elif defined (CSL_PLATFORM_MIPS)
    return "MIPS";
#elif defined (CSL_PLATFORM_PPC)
#   ifdef CSL_PLATFORM_PPC64 
        return "PowerPC64";
#   else 
        return "PowerPC";
#   endif
#elif defined (CSL_PLATFORM_UNKNOWNARCH)
    return "Unknown Architecture";
#endif
}

/*************/
/* Compilers */
/*************/

#if defined (__GNUC__)
#   define CSL_PLATFORM_GCC
#elif defined (_MSC_VER)
#   define CSL_PLATFORM_MSVC
#elif defined (__CYGWIN__)
#   define CSL_PLATFORM_CYGWIN
#elif defined (__MINGW32__)
#   define CSL_PLATFORM_MINGW
#else
#   define CSL_PLATFORM_UNKNOWNCC
#endif

static const char *csl_platform_compiler (void) {
#if defined (CSL_PLATFORM_GCC)
    return "GNU C Compiler";
#elif defined (CSL_PLATFORM_MSVC)
    return "Microsoft Visual C++ Compiler";
#elif defined (CSL_PLATFORM_CYGWIN)
    return "Cygwin Compiler";
#elif defined (CSL_PLATFORM_MINGW)
    return "MingGW32 Compiler";
#elif defined (CSL_PLATFORM_UNKNOWNCC)
    return "Unknown C Compiler";
#endif
}

#endif
