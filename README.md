# CSL

**CSL** or Ce**S**sentia**L**s is a collection of headers that I find nice to have in my daily C coding routine.

As for now this only includes a simple header-only vector implementation - pvector. But as this collection grows, new headers will appear here.

## pvector

pvector is a header-only implementation of a dynamically-resizable array, which is, in C++ terms, called a vector.

To create a vector, simply type:

```c
    int   *integer_vector = csl_pvector (int); /* this will create a vector storage for integers */
    void **pointer_vector = csl_pvector (void *); /* this will create a vector storage for pointers */
```

Other functions can be found in the `pvector/pvector.h` header file.
