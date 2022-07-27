/*!
\file matrix.c
\author Edward Ji
\date 21 Jun 2022

\brief
A demonstration of representing a matrix with nested dynamic arrays and printing
it neatly.

\stdout
```
[   71876166  708592740 1483128881  907283241 ]
[  442951012  537146758 1366999021 1854614940 ]
[  647800535   53523743  783815874 1643643143 ]
```
*/

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../darray.h"

//! The number of rows in the sample matrix.
#define N_ROWS 3
//! The number of columns in the sample matrix.
#define N_COLS 4

//! The field width when printing out an integer entry.
static int field_width;

//! Allocates an integer on the heap.
int *new_int(int x) {
    int *p = (int *) malloc(sizeof(int));
    *p = x;

    return p;
}

/*!
Assigns the integer at the first pointer if it's larger than that at the
second pointer.

\see This function is of type `aggregate`.
*/
void int_max(const void *p1, void *p2) {
    const int *item = p1;
    int *resp = p2;
    if (*item > *resp) {
        *resp = *item;
    }
}

/*!
Assigns the max integer in the array at the first pointer if it's larger than
that at the second pointer.

\see This function is of type `aggregate`.
*/
void array_max(const void *p1, void *p2) {
    darray *arr = (darray *) p1;
    int *resp = p2;
    int max = INT_MIN;
    darray_aggregate(arr, &max, int_max);
    if (max > *resp) {
        *resp = max;
    }
}

/*!
Prints the integer at a pointer with global field width followed by a space.

\see This function is of type `consumer`.
*/
void print_int(void *p) {
    printf("%*d ", field_width, *((int *) p));
}

/*!
Prints the integer array at a pointer in the following format.
```
[   71876166  708592740 1483128881  907283241 ]
```

\see This function is of type `consumer`.
*/
void print_arr(void *arr) {
    printf("[ ");
    darray_foreach((darray *) arr, print_int);
    printf("]\n");
}


/*!
Sets the global field width to the number of digits of the max integer in the
matrix and prints the nested integer array at a pointer.

\see This function is of type `consumer`.
*/
void print_mat(darray *arr) {
    int max = 0;
    darray_aggregate(arr, &max, array_max);
    field_width = (int) ceil(log10((double) max));
    darray_foreach(arr, print_arr);
}

int main() {
    srand(42);

    darray *mat = new_darray((consumer) del_darray);
    for (int i = 0; i < N_ROWS; i++) {
        darray *row = new_darray(free);
        for (int j = 0; j < N_COLS; j++) {
            int num = rand();
            darray_append(row, new_int(num));
        }
        darray_append(mat, row);
    }

    print_mat(mat);

    del_darray(mat);

    return 0;
}
