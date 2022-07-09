/* strings.h - fill in the gaps of lacking str[n]casecmp and strdup on *certain* OSes */
/* Sergey Lafin, 2022 */

#ifndef __CSL_STRINGS_H__
#define __CSL_STRINGS_H__

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include <errno.h>

/* this header defines the following functions 
 * - strcasecmp 
 * - strncasecmp 
 * - strdup 
 * - strndup */

/* define a CSL_STRINGS_REPLACE macro before inclusion to replace the default
 * functions (if they are present) */

#ifndef CSL_MALLOC
#   include <stdlib.h>
#   define CSL_MALLOC malloc
#endif

#if !(defined (__clang__) || defined (__CYGWIN__) || defined (__unix__) || defined (_GNU_SOURCE)) || defined (CSL_STRINGS_REPLACE)
#   define NEED_STRCASECMP 1
#   define NEED_STRDUP     1
#endif

#if NEED_STRCASECMP
    static int csl_strcasecmp (const char *s1, const char *s2) {
        int c1, c2;
        while (*s1 && (c1 = tolower (*s1)) == (c2 = tolower(*s2)))
            ++s1, ++s2;

        return (c1 > c2) - (c2 > c1);
    }

    static int csl_strncasecmp (const char *s1, const char *s2, size_t n) {
        int c1, c2;
        while (n-- && *s1 && (c1 = tolower (*s1)) == (c2 = tolower (*s2)))
            ++s1, ++s2;

        return (c1 > c2) - (c2 > c1);
    }

/* to avoid redefintions */
#   define strcasecmp  csl_strcasecmp
#   define strncasecmp csl_strncasecmp
#endif

#if NEED_STRDUP
    static char *csl_strdup (const char *str) {
        size_t slen = strlen (str) + 1;
        char *ns = CSL_MALLOC (slen);
        if (!ns) {
            errno = ENOMEM;
            return NULL;
        }
        memcpy (ns, str, slen);
        return ns;
    }
        
    static char *csl_strndup (const char *str, size_t n) {
        if (!n++)
            return NULL;
        size_t slen  = strlen (str) + 1;
        size_t nsize = slen > n ? n : slen + 1;
        char *ns = CSL_MALLOC (nsize);
        if (!ns) {
            errno = ENOMEM;
            return NULL;
        }
        memcpy (ns, str, nsize - 1);
        ns[nsize - 1] = '\0';
        return ns;
    }

/* to avoid redefintions */
#   define strdup csl_strdup
#   define strndup csl_strndup
#endif

#undef HAVE_STRDUP
#undef HAVE_STRCASECMP

#endif
