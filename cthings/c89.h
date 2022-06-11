/* c89.h - compatibility layer with newer standards of C */
/* Sergey Lafin, 2022 */

#ifndef __CSL_C89_H__
#define __CSL_C89_H__

#if defined (__cplusplus) || __STDC_VERSION__ >= 199901L
#   define CSL_INLINE static inline 
#   ifdef __STDC_VERSION__
#       define CSL_BOOL _Bool
#   else 
#       define CSL_BOOL bool 
#   endif
#else 
#   define CSL_INLINE static 
#   define CSL_BOOL   int
#endif

#define CSL_TRUE   1
#define CSL_FALSE  0

#endif
