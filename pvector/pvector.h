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

#ifndef CSL_INLINE
#   if __CSL_CVER__ >= 1999 
#       define CSL_INLINE static inline 
#   else 
#       define CSL_INLINE static
#   endif
#endif

#ifndef CSL_PVECTOR_USE_LONG
#   define CSL_PVECTOR_SIZE unsigned
#   define CSL_PVECTOR_PAD  char __pad[4];
#else
#   define CSL_PVECTOR_SIZE size_t
#   define CSL_PVECTOR_PAD  char __pad[12];
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
    
        CSL_NORETURN CSL_INLINE void __csl_assert_failed (const char *expr, const char *message, const char *function) {
            fprintf (stderr, "%s: assertion %s failed: %s\n", function, expr, message);
            abort ();
        }
#   else
#       define CSL_ASSERT(expr, str)
#   endif 
#endif

/* header definition */ 
typedef struct csl_pvector_header {
    /* number of elements currently stored in the array */
    CSL_PVECTOR_SIZE count;
    /* size of the allocated block for the array */
    CSL_PVECTOR_SIZE capacity;
    /* size of a single element */
    unsigned e_size;

    /* padding to make the array aligned */
    CSL_PVECTOR_PAD
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

CSL_INLINE void *__csl_pvector_alloc (unsigned T_size) {
    CSL_ASSERT (T_size > 0, "Element size is expected to be non-zero");

    csl_pvector_t *p = (csl_pvector_t *)malloc (sizeof (csl_pvector_t));
    if (!p)
        return NULL;

    p->capacity = 0;
    p->count    = 0;
    p->e_size   = T_size;

    return (char *)p + sizeof (csl_pvector_t);
}

CSL_INLINE void *__csl_pvector_resize (csl_pvector_t *v, CSL_PVECTOR_SIZE size) {
    v->capacity = size;

    v = (csl_pvector_t *)realloc (v, v->e_size * size + sizeof (csl_pvector_t));
    CSL_ASSERT (v != NULL, "failed to reallocate array");

    return (char *)v + sizeof (csl_pvector_t);
} 

CSL_INLINE void __csl_pvector_reserve (csl_pvector_t *v, CSL_PVECTOR_SIZE new_cap) {
    CSL_ASSERT (new_cap >= v->capacity, "Expected new_cap to be equal or greater than the current capacity");

    v = (csl_pvector_t *)realloc (v, v->e_size * new_cap + sizeof (csl_pvector_t));
    CSL_ASSERT (v != NULL, "failed to reallocate array");

    v->capacity = new_cap;
}

CSL_INLINE void *__csl_pvector_resize_if_needed (csl_pvector_t *v, CSL_PVECTOR_SIZE size) {
    /* resize to twice the size */
    size_t new_cap = size * 2;

    if (size > v->capacity) {
        v = (csl_pvector_t *)realloc (v, v->e_size * new_cap + sizeof (csl_pvector_t));
        v->capacity = new_cap;
    }

    return (char *)v + sizeof (csl_pvector_t);
}

CSL_INLINE void __csl_pvector_vmove (csl_pvector_t *v, unsigned addr, signed off) {
    if (abs (off) > v->count)
        return;
    memmove (__CSL_ARRAYOF (v) + off * v->e_size, __CSL_ARRAYOF (v) + (addr * v->e_size), v->count * v->e_size);
}

CSL_INLINE void *__csl_pvector_copy (csl_pvector_t *v) {
    size_t blk_size = sizeof (csl_pvector_t) + v->count * v->e_size;
    csl_pvector_t *blk = (csl_pvector_t *)malloc (blk_size);
    memcpy (blk, v, blk_size);

    blk->capacity = v->capacity;

    return (char *)blk + sizeof (csl_pvector_t);
}

CSL_INLINE void __csl_pvector_remove (csl_pvector_t *v, CSL_PVECTOR_SIZE from, CSL_PVECTOR_SIZE size) {
    CSL_ASSERT (size != 0 && size < v->count, "size of the removed region can't exceed size of the array");

    CSL_PVECTOR_SIZE to = from + size;
    CSL_ASSERT (size < v->capacity && to < v->capacity, "size should be greater than vector capacity");
    
    memmove (__CSL_ARRAYOF (v) + from * v->e_size, __CSL_ARRAYOF (v) + to * v->e_size, v->count * v->e_size - v->e_size * size);
    v->count -= size;
}

#undef CSL_PVECTOR_PAD
#undef CSL_PVECTOR_SIZE

#endif
