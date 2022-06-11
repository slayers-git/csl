/* attribute.h - compiler-independent attributes */
/* Sergey Lafin, 2022 */

#ifndef __CSL_ATTRIBUTE_H__
#define __CSL_ATTRIBUTE_H__

#if defined (__GNUC__) || defined (__MINGW32__) || defined (__CYGWIN__)
#   define CSL_NORETURN   __attribute__((noreturn))
#   define CSL_NOINLINE   __attribute__((noinline))

#   define CSL_PACKED(__p_attribute_) __p_attribute_ __attribute__((__packed__))
#   define CSL_BEGIN_PACKED
#   define CSL_END_PACKED   __attribute__((__packed__))
#elif defined (_MSC_VER)
#   define CSL_NORETURN   __declspec(noreturn)
#   define CSL_NOINLINE   __declspec(noinline)

#   define CSL_PACKED(__p_attribute_) \
    __pragma (pack(push, 1)) \
    __p_attribute_           \
    __pragma (pack(pop))
#   define CSL_BEGIN_PACKED  \
    __pragma (pack(push, 1))
#define CSL_END_PACKED \
    __pragma (pack(pop))
#else
#   error "csl attribute.h is not supported for this compiler"
#endif

#endif 
