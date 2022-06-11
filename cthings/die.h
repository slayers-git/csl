#ifndef __CSL_DIE_H__
#define __CSL_DIE_H__

#include <stdio.h>
#include <stdlib.h>

#ifndef CSL_DIE_STREAM 
#   define CSL_DIE_STREAM stderr
#endif

/* abort the execution leaving the message in CSL_DIE_STREAM */
#define csl_die(msg) \
    { fprintf (CSL_DIE_STREAM, "%s\n", msg); abort (); } 

#define csl_vdie(fmt, ...) \
    { fprintf (CSL_DIE_STREAM, fmt "\n", __VA_ARGS__); abort (); }

#endif 
