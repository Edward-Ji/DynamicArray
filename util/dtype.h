/*!
\file util/dtype.h
\author Edward Ji
\date 26 Jul 2022
\brief Some utility macros for generating wrapper functions around C data types.
*/

#include <stdio.h>
#include <stdlib.h>

#include "../darray.h"

#define MAKE_DTYPE(type, alph, fmt)                                            \
                                                                               \
type *new_##alph(type x) {                                                     \
    type *p = (type *) malloc(sizeof(type));                                   \
    *p = x;                                                                    \
    return p;                                                                  \
}                                                                              \
                                                                               \
void print_##alph(void *p1) {                                                  \
    printf(fmt, *((type *) p1));                                               \
}                                                                              \
                                                                               \
int alph##_cmp(const void *p1, const void *p2) {                               \
    if (p1 == NULL || p2 == NULL) return 0;                                    \
    const type *tp1 = (const type *) p1;                                       \
    const type *tp2 = (const type *) p2;                                       \
    return (*tp1 > *tp2) - (*tp1 < *tp2);                                      \
}                                                                              \
                                                                               \
void *alph##_cpy(const void *p) { return (void *) p; }                         \
                                                                               \
void *alph##_cpy_deep(const void *p) {                                         \
    type *cpy = (type *) malloc(sizeof(type));                                 \
    *cpy = *((type *) p);                                                      \
    return cpy;                                                                \
}

#define MAKE_DTYPE_CHAR     MAKE_DTYPE(char,               char,     "%c ")
#define MAKE_DTYPE_SCHAR    MAKE_DTYPE(signed char,        schar,    "%hhi ")
#define MAKE_DTYPE_UCHAR    MAKE_DTYPE(unsigned char,      uchar,    "%hhu ")
#define MAKE_DTYPE_SHORT    MAKE_DTYPE(short,              short,    "%hi ")
#define MAKE_DTYPE_USHORT   MAKE_DTYPE(unsigned short,     ushort,   "%hu ")
#define MAKE_DTYPE_INT      MAKE_DTYPE(int,                int,      "%d ")
#define MAKE_DTYPE_UNSIGNED MAKE_DTYPE(unsigned,           unsigned, "%u ")
#define MAKE_DTYPE_LONG     MAKE_DTYPE(long,               long,     "%ld ")
#define MAKE_DTYPE_ULONG    MAKE_DTYPE(unsigned long,      ulong,    "%lu ")
#define MAKE_DTYPE_LLONG    MAKE_DTYPE(long long,          llong,    "%lld ")
#define MAKE_DTYPE_ULLONG   MAKE_DTYPE(unsigned long long, ullong,   "%llu ")
#define MAKE_DTYPE_FLOAT    MAKE_DTYPE(float,              float,    "%g ")
#define MAKE_DTYPE_DOUBLE   MAKE_DTYPE(double,             double,   "%lg ")
#define MAKE_DTYPE_LDOUBLE  MAKE_DTYPE(long double,        ldouble,  "%Lg ")
