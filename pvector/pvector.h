/* pvector - header only vector implementation in C */
/* Sergey Lafin, 2022 */

#ifndef __CSL_VECTOR_H__
#define __CSL_VECTOR_H__

#include <stddef.h>
#include <assert.h>

#include <stdlib.h>
#include <string.h>

/************************/
/* standard definitions */
/************************/

#ifndef __CSL_CVER__
#   if __STDC_VERSION__ == 199901L
#       define __CSL_CVER__ 1999
#   elif __STDC_VERSION__ <= 199401L
#       define __CSL_CVER__ 1989
#   elif __STDC_VERSION__ >= 201112L
#       define __CSL_CVER__ 2011
#   endif
#endif 

#ifndef __CSL_INLINE__
#   if __CSL_CVER__ >= 1999 
#       define __CSL_INLINE__ static inline 
#   else 
#       define __CSL_INLINE__ static
#   endif
#endif

#ifndef __CSL_BOOL__ 
#   define __CSL_BOOL__
#   if __CSL_CVER__ >= 1999
#       include <stdbool.h>
        typedef _Bool csl_bool;
#   else 
        typedef char  csl_bool;
#       define true  1
#       define false 0
#   endif
#endif

#ifndef __CSL_RESULT__
#   define __CSL_RESULT__
    typedef int csl_result;
#endif

#ifndef __CSL_PVECTOR_USE_LONG__
#   define __CSL_SIZE__ unsigned
#   define __CSL_PVECTOR_PAD__  char __pad[4];
#else
#   define __CSL_SIZE__ size_t
#   define __CSL_PVECTOR_PAD__  char __pad[12];
#endif

#ifndef __CSL_ALIGNED__
#   if defined (__GNUC__) || defined (__clang__)
#       define __CSL_ALIGNED__(x) __attribute__((aligned(x)))
#   elif defined (_MSC_VER)
#       define __CSL_ALIGNED__(x) __declspec(align(x))
#   endif
#endif

#ifndef __CSL_NORETURN__
#   if defined (__GNUC__) || defined (__clang__)
#       define __CSL_NORETURN__ __attribute__((noreturn))
#   elif defined (_MSC_VER)
#       define __CSL_NORETURN__ __declspec(noreturn)
#   endif
#endif

#ifndef __CSL_ASSERT__
#   ifndef __CSL_NOASSERT__
#       define __CSL_ASSERT__(expr, str) { if (!(expr)) {__csl_assert_failed (#expr, str, __FUNCTION__); } }
#       include <stdio.h>
    
        __CSL_NORETURN__ __CSL_INLINE__ void __csl_assert_failed (const char *expr, const char *message, const char *function) {
            fprintf (stderr, "%s: assertion %s failed: %s\n", function, expr, message);
            abort ();
        }
#   else
#       define __CSL_ASSERT__(expr, str)
#   endif 
#endif

/* header definition */
__CSL_ALIGNED__(16) typedef struct csl_pvector_header {
    /* number of elements currently stored in the array */
    __CSL_SIZE__ count;
    /* size of the allocated block for the array */
    __CSL_SIZE__ capacity;
    /* size of a single element */
    unsigned e_size;

    /* padding to make the array aligned */
    __CSL_PVECTOR_PAD__
} csl_pvector_t;

/******************/
/* Main functions */
/******************/

/* allocate a vector of type T */
#define csl_pvector(T)\
    ((T *)__csl_pvector_alloc (sizeof (T)))

/* get the size of a vector */
#define csl_pvector_size(v)\
    (((csl_pvector_t *)((char *)v - sizeof (csl_pvector_t)))->count)

/* free the vector */
#define csl_pvector_free(v) \
    free ((void *)v - sizeof (csl_pvector_t))

/* push an element back to the tail */
#define csl_pvector_push_back(v, val) {\
    csl_pvector_t *hdr = __CSL_HEADEROF (v);\
    v = __csl_pvector_resize_if_needed (hdr, hdr->count + 1);\
    hdr = __CSL_HEADEROF (v);\
    v[hdr->count++] = val;\
}

/* push an element in front of a vector */
#define csl_pvector_push_front(v, val) {\
    csl_pvector_t *hdr = __CSL_HEADEROF (v);\
    v = __csl_pvector_resize_if_needed (hdr, ++hdr->count);\
    __csl_pvector_vmove  (__CSL_HEADEROF(v), 0, 1);\
    v[0] = val;\
}

/* remove an element from the back of the vector */
#define csl_pvector_pop_back(v) {\
    csl_pvector_t *hdr = __CSL_HEADEROF (v);\
    if (hdr->count) {\
        --hdr->count;\
    }\
}

/* remove an element from the beginning of the vector */
#define csl_pvector_pop_front(v) {\
    csl_pvector_t *hdr = __CSL_HEADEROF (v);\
    if (hdr->count) {\
        __csl_pvector_vmove (hdr, 1, 0);\
        --hdr->count;\
    }\
}

/* reserve a space for the array to avoid dynamic allocations */
#define csl_pvector_reserve(v, s) {\
    csl_pvector_t *hdr = __CSL_HEADEROF (v);\
    __csl_pvector_reserve (hdr, (unsigned)s);\
}

/* resize the array so it fits the block completely */
#define csl_pvector_shrink_to_fit(v) {\
    csl_pvector_t *hdr = __CSL_HEADEROF (v); \
    v = __csl_pvector_resize (hdr, hdr->count); \
}

/* clone the vector into other variable */
#define csl_pvector_copy(v)\
    (__csl_pvector_copy (__CSL_HEADEROF (v)))

/* get the capacity of a vector */
#define csl_pvector_capacity(v) \
    (((csl_pvector_t *)((void *)v - sizeof (csl_pvector_t)))->capacity)

#define __CSL_ARRAYOF(hdr)\
    ((char *)hdr + sizeof (csl_pvector_t))

#define __CSL_HEADEROF(v)\
    ((csl_pvector_t *)((char *)v - sizeof (csl_pvector_t)))

/* remove an area of a vector */
#define csl_pvector_remove(v, from, size) \
    (__csl_pvector_remove ((csl_pvector_t *)((char *)(v) - sizeof (csl_pvector_t)), (from), (size)))

/* clear the vector */
#define csl_pvector_clear(v) \
    { ((csl_pvector_t *)((char *)(v) - sizeof (csl_pvector_t)))->count = 0; }

/****************/
/* INTERNAL USE */
/****************/

__CSL_INLINE__ void *__csl_pvector_alloc (unsigned T_size) {
    __CSL_ASSERT__ (T_size > 0, "Element size is expected to be non-zero");

    csl_pvector_t *p = (csl_pvector_t *)malloc (sizeof (csl_pvector_t));
    if (!p)
        return NULL;

    p->capacity = 0;
    p->count    = 0;
    p->e_size   = T_size;

    return (char *)p + sizeof (csl_pvector_t);
}

__CSL_INLINE__ void *__csl_pvector_resize (csl_pvector_t *v, __CSL_SIZE__ size) {
    v->capacity = size;

    v = (csl_pvector_t *)realloc (v, v->e_size * size + sizeof (csl_pvector_t));
    __CSL_ASSERT__ (v != NULL, "failed to reallocate array");

    return (char *)v + sizeof (csl_pvector_t);
} 

__CSL_INLINE__ void __csl_pvector_reserve (csl_pvector_t *v, __CSL_SIZE__ new_cap) {
    __CSL_ASSERT__ (new_cap >= v->capacity, "Expected new_cap to be equal or greater than the current capacity");

    v = (csl_pvector_t *)realloc (v, v->e_size * new_cap + sizeof (csl_pvector_t));
    __CSL_ASSERT__ (v != NULL, "failed to reallocate array");

    v->capacity = new_cap;
}

__CSL_INLINE__ void *__csl_pvector_resize_if_needed (csl_pvector_t *v, __CSL_SIZE__ size) {
    /* resize to twice the size */
    size_t new_cap = size * 2;

    if (size > v->capacity) {
        v = (csl_pvector_t *)realloc (v, v->e_size * new_cap + sizeof (csl_pvector_t));
        v->capacity = new_cap;
    }

    return (char *)v + sizeof (csl_pvector_t);
}

__CSL_INLINE__ void __csl_pvector_vmove (csl_pvector_t *v, unsigned addr, signed off) {
    if (abs (off) > v->count)
        return;
    memmove (__CSL_ARRAYOF (v) + off * v->e_size, __CSL_ARRAYOF (v) + (addr * v->e_size), v->count * v->e_size);
}

__CSL_INLINE__ void *__csl_pvector_copy (csl_pvector_t *v) {
    size_t blk_size = sizeof (csl_pvector_t) + v->count * v->e_size;
    csl_pvector_t *blk = (csl_pvector_t *)malloc (blk_size);
    memcpy (blk, v, blk_size);

    blk->capacity = v->capacity;

    return (char *)blk + sizeof (csl_pvector_t);
}

__CSL_INLINE__ void __csl_pvector_remove (csl_pvector_t *v, __CSL_SIZE__ from, __CSL_SIZE__ size) {
    __CSL_ASSERT__ (size != 0 && size < v->count, "size of the removed region can't exceed size of the array");

    __CSL_SIZE__ to = from + size;
    __CSL_ASSERT__ (size < v->capacity && to < v->capacity, "size should be greater than vector capacity");
    
    memmove (__CSL_ARRAYOF (v) + from * v->e_size, __CSL_ARRAYOF (v) + to * v->e_size, v->count * v->e_size - v->e_size * size);
    v->count -= size;
}

#endif
