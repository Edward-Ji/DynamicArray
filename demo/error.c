/*!
\file error.c
\author Edward Ji
\date 24 Jul 2022

\brief A demonstration of error handling with dynamic array.

\stdout
```
error 3: invalid index
```
*/
#include <stdio.h>
#include <stdlib.h>

#include "../darray.h"

int main() {
    /* First, create an empty array. */
    darray *vec = new_darray(free);

    /* Next, declare and initialize an integer to insert. */
    int x = 42;

    /* Attempt to insert at index 1 in an empty array. */
    if (darray_insert(vec, 1, &x) == 0) {
        /* The return code is zero indicating an error. */
        printf("error %d: %s\n", darray_errno, darray_strerr());
    }

    /* Last but not least, clean up the array before exit. */
    del_darray(vec);

    return 0;
}
