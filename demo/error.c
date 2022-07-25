#include <stdio.h>
#include <stdlib.h>

#include "../darray.h"

int main(void) {
    darray *vec = new_darray(free);

    int x = 42;

    if (darray_insert(vec, 1, &x) == 0) {
        printf("darray error: %s\n", darray_strerr());
    }

    del_darray(vec);

    return 0;
}
