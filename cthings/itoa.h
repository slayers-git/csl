/* itoa.h - compiler-independent itoa implementation */
/* Sergey Lafin, 2022 */

#ifndef __CSL_ITOA_H__
#define __CSL_ITOA_H__

#ifndef __CSL_CVER__
#   if __STDC_VERSION__ == 199901L
#       define __CSL_CVER__ 1999
#   elif __STDC_VERSION__ <= 199401L
#       define __CSL_CVER__ 1989
#   elif __STDC_VERSION__ >= 201112L
#       define __CSL_CVER__ 2011
#   endif
#endif 

#ifndef CSL_INLINE
#   if __CSL_CVER__ >= 1999 
#       define CSL_INLINE static inline 
#   else 
#       define CSL_INLINE static
#   endif
#endif

#ifndef CSL_NORETURN
#   if defined (__GNUC__) || defined (__clang__)
#       define CSL_NORETURN __attribute__((noreturn))
#   elif defined (_MSC_VER)
#       define CSL_NORETURN __declspec(noreturn)
#   else
#       define CSL_NORETURN
#   endif
#endif

#ifndef CSL_ASSERT
#   ifndef __CSL_NOASSERT__
#       define CSL_ASSERT(expr, str) { if (!(expr)) {__csl_assert_failed (#expr, str, __FUNCTION__); } }
#       include <stdio.h>
#       include <stdlib.h>
    
        CSL_NORETURN CSL_INLINE void __csl_assert_failed (const char *expr, const char *message, const char *function) {
            fprintf (stderr, "%s: assertion %s failed: %s\n", function, expr, message);
            abort ();
        }
#   else
#       define CSL_ASSERT(expr, str)
#   endif 
#endif

#ifdef CSL_ITOA_REPLACE
#   define itoa csl_itoa
#endif

#ifdef CSL_ITOA_LONG
#   define __csl_itoa_int  long
#   define __csl_itoa_uint unsigned long
#else
#   define __csl_itoa_int  int
#   define __csl_itoa_uint unsigned
#endif

/* Posix-compliant itoa implementation */
CSL_INLINE char *csl_itoa (__csl_itoa_int value, char *buffer, unsigned base) {
    CSL_ASSERT (base >= 2 && base <= 36, "csl_itoa () expects the base to be in [2, 36] range");

    if (!value) {
        buffer[0] = '0';
        buffer[1] = '\0';

        return buffer;
    }

    int neg = (base == 10 && value < 0);
    __csl_itoa_uint val = neg ? -value : value;

    static const char *al = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    unsigned i, j;
    for (i = 0; val; ++i) {
        buffer[i] = al[val % base];
        val /= base;
    }

    if (neg)
        buffer[i++] = '-';

    for (j = 0; j < i / 2; ++j) {
        char t = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = t;
    }

    buffer[i] = '\0';

    return buffer;
}

#endif
