/* standard.h - helper to determine the standard the compiler uses 
 * to generate the code. */
/* Sergey Lafin, 2020 */

#ifndef __CSL_STANDARD_H__
#define __CSL_STANDARD_H__

#if (defined (__STDC_VERSION__) || defined (__STDC__)) && !defined (__cplusplus)
#   if __STDC_VERSION__ == 199901L
#       define CSL_STANDARD_C99
#       define CSL_STANDARD_C 99
#   elif __STDC_VERSION__ == 199401L
#       define CSL_STANDARD_C90
#       define CSL_STANDARD_C 90
#   elif __STDC_VERSION__ == 201112L
#       define CSL_STANDARD_C11
#       define CSL_STANDARD_C 11
#   elif __STDC_VERSION__ == 201710L
#       define CSL_STANDARD_C17
#       define CSL_STANDARD_C 17
#   else
#       define CSL_STANDARD_C89 
#       define CSL_STANDARD_C 89
#       define CSL_STANDARD_UNKNOWN
#   endif
#elif defined (__cplusplus)
#   if __cplusplus == 199711L
#       define CSL_STANDARD_CPP98
#       define CSL_STANDARD_CPP03
#       define CSL_STANDARD_CPP 98
#   elif __cplusplus == 201103L
#       define CSL_STANDARD_CPP11
#       define CSL_STANDARD_CPP 11
#   elif __cplusplus == 201402L
#       define CSL_STANDARD_CPP14
#       define CSL_STANDARD_CPP 14
#   elif __cplusplus == 201703L
#       define CSL_STANDARD_CPP17
#       define CSL_STANDARD_CPP 17
#   elif __cplusplus == 202002L
#       define CSL_STANDARD_CPP20
#       define CSL_STANDARD_CPP 20
#   else 
#       define CSL_STANDARD_CPP98
#       define CSL_STANDARD_CPP03
#       define CSL_STANDARD_CPP 98
#       define CSL_STANDARD_UNKNOWN
#   endif
#else
#   define CSL_STANDARD_UNKNOWN
#endif

#endif 
