# CSL

**CSL** or **C**e**S**sentia**L**s is a collection of headers that I find nice to have in my daily C coding routine.

Currently, this collection includes a simple header-only vector implementation - pvector, and some useful C header files located in the `cthings` directory.

## pvector

pvector is a header-only implementation of a dynamically-resizable array, which is, in C++ terms, called a vector.

To create a vector, simply type:

```c
int   *integer_vector = csl_pvector (int); /* this will create a vector storage for integers */
void **pointer_vector = csl_pvector (void *); /* this will create a vector storage for pointers */
```

Other functions can be found in the `pvector/pvector.h` header file.

## cthings

cthings directory is comprised of the miscellaneous, independent from each other header files you can find useful for coding in C. As for now this includes:

- `platform.h` - a header file to determine the host architecture, OS and the compiler used to assemble the program.
- `standard.h` - a header file to determine the compiler's C/C++ standard.
- `attribute.h` - a header file to aid cross-compiler attribute problem.
- `begin_code.h` - a header file to help enclose C code in, while saving compatibility with C++ (extern "C").
- `c89.h` - a header file for c99 support in older C standards.
- `die.h` - a simple die () and vdie () implementation.
- `itoa.h` - a simple POSIX itoa implementation.
- `hash.h` - a collection of string hash functions.
- `strings.h` - a header file to add some functions from POSIX's `strings.h` header file to non-POSIX platforms (`str[n]casecmp ()`, `str[n]dup ()` functions specifically).
- `getline.h` - a slightly modified version of POSIX's `getdelim ()` and `getline ()` functions.

## Installing

All sources in csl library are headers. So installing them is simply a matter of:

```bash
$ cmake .
# make install
```
