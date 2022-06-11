/* begin_code.h - simple C code enclosure for C++ compilers */
/* Sergey Lafin, 2022 */

#ifndef __CSL_BEGIN_CODE_H__
#define __CSL_BEGIN_CODE_H__

#ifdef __cplusplus
#   define CSL_BEGIN_CODE extern "C" {
#else
#   define CSL_BEGIN_CODE
#endif

#ifdef __cplusplus 
#   define CSL_END_CODE }
#else
#   define CSL_END_CODE 
#endif

#endif
