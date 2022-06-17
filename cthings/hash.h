/* hash.h - some hash functions for strings */
/* Sergey Lafin, 2022 */

#ifndef __CSL_HASH_H__
#define __CSL_HASH_H__

#include <stdint.h>
#include <ctype.h>
#include <string.h>

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

#ifndef CSL_PREFERRED_HASH_ALGO
#   define CSL_PREFERRED_HASH_ALGO csl_djb_hash
#endif

#ifndef CSL_HASH_SEED
#   define CSL_HASH_SEED 0x0A023DB
#endif

typedef uint64_t csl_hash_t;

/**********************/
/* Hashing algorithms */
/**********************/

/* djb hash */
static csl_hash_t csl_djb_hash (const char *name, unsigned seed) {
    char c;
    csl_hash_t h, s = seed % sizeof (csl_hash_t);
    for (h = 0; *name; ++name) {
        c = toupper (*name);
        h = ((h << s) + h) ^ c;
    }

    return h;
}

/* murmur 64-bit */
#ifndef CSL_HASH_NOMURMUR
    /* slightly changed version from https://sites.google.com/site/murmurhash */
    static csl_hash_t csl_murmur2_hash (const char *str, unsigned seed) {
        unsigned long len = strlen (str);

        const unsigned m = 0x5bd1e995;
        const int r = 24;

        unsigned h1 = seed ^ len;
        unsigned h2 = 0;

        const unsigned *data = (const unsigned *)str;

        while (len >= 8) {
            unsigned k1 = *data++;
            k1 *= m; k1 ^= k1 >> r; k1 *= m;
            h1 *= m; h1 ^= k1;
            len -= 4;

            unsigned k2 = *data++;
            k2 *= m; k2 ^= k2 >> r; k2 *= m;
            h2 *= m; h2 ^= k2;
            len -= 4;
        }

        if (len >= 4) {
            unsigned k1 = *data++;
            k1 *= m; k1 ^= k1 >> r; k1 *= m;
            h1 *= m; h1 ^= k1;
            len -= 4;
        }

        switch (len) {
        case 3: h2 ^= ((unsigned char *)data)[2] << 16;
        case 2: h2 ^= ((unsigned char *)data)[1] << 8;
        case 1: h2 ^= ((unsigned char *)data)[0];
                h2 *= m;
        };

        h1 ^= h2 >> 18; h1 *= m;
        h2 ^= h1 >> 22; h2 *= m;
        h1 ^= h2 >> 17; h1 *= m;
        h2 ^= h1 >> 19; h2 *= m;

        csl_hash_t h = h1;

        h = (h << 32) | h2;

        return h;
    } 
#endif

#define csl_strhash(str) \
    (CSL_PREFERRED_HASH_ALGO (str, CSL_HASH_SEED))

#define csl_strhash_seed(str, seed) \
    (CSL_PREFERRED_HASH_ALGO (str, seed))

#endif
