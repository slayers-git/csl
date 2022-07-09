/* getline.h - simple implementation of getdelim () and getline () functions */

#ifndef __CSL_GETLINE_H__
#define __CSL_GETLINE_H__

/* for NULL and ssize_t */
#include <stddef.h>
/* for FILE definition and getc () */
#include <stdio.h>

/* for SSIZE_MAX */
#include <limits.h>

#include <errno.h>

/* XXX could use CSL_REALLOC (NULL, n) for the same result */
#ifndef CSL_MALLOC
#   include <stdlib.h>
#   define CSL_MALLOC malloc
#endif 

#ifndef CSL_REALLOC
#   include <stdlib.h>
#   define CSL_REALLOC realloc
#endif

#ifdef CSL_GETLINE_REPLACE
#   define getdelim csl_getdelim
#   define getline csl_getline
#endif

#define CSL_GETLINE_EOF -256

/* Unlike the POSIX variant, this getdelim function doesn't append a delim character 
 * if it was found in the stream. 
 *
 * Returns the amount of characters consumed, otherwise -1 or -256 (CSL_GETLINE_EOF).
 *
 * If an EOF was met, then, if the line is empty, the function will return CSL_GETLINE_EOF, 
 * otherwise it will return the amount of characters consumed before the EOF was met.
 *
 * This function grows the buffer exponentially. */
static ssize_t csl_getdelim (char **lineptr, size_t *n, int delim, FILE *stream) {
    if (!lineptr || !n || (*lineptr && !*n) || !stream) {
        errno = EINVAL;
        return -1;
    }

    errno = 0;

    char *p;
    if (*lineptr) {
        p = *lineptr;
    } else {
        *n = 128;
        p = *lineptr = CSL_MALLOC (*n);
        if (!p) {
            errno = ENOMEM;
            return -1;
        }
    }

    int c;
    size_t consumed = 0;
    for (; (c = getc (stream)) != delim; ++p, ++consumed) {
        if (c == EOF)
            return consumed ? consumed : CSL_GETLINE_EOF;

        if (consumed > SSIZE_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        if (consumed >= *n - 1) {
            *n *= 2;
            *lineptr = CSL_REALLOC (*lineptr, *n);
            if (!*lineptr) {
                errno = ENOMEM;
                return -1;
            }
            
            p = *lineptr + consumed;
        }

        *p = c;
    }

    *p = '\0';
    return consumed;
}

/* XXX Doesn't append the '\n' symbol to the end of the buffer line */
static ssize_t csl_getline (char **lineptr, size_t *n, FILE *stream) {
    return csl_getdelim (lineptr, n, '\n', stream);
}

#endif
