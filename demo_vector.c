#include <stdio.h>
#include <stdlib.h>

#include "darray.h"

int *new_int(int num) {
    int *intp = (int *) malloc(sizeof(int));
    *intp = num;

    return intp;
}

void print_int(int *intp) {
    printf("%d ", *intp);
}

int main() {
    darray *vec1 = new_darray(free);
    darray *vec2 = new_darray(NULL);

    darray_append(vec1, new_int(2));
    darray_append(vec1, new_int(3));
    darray_append(vec1, new_int(4));

    darray_insert(vec2, 0, new_int(1));
    darray_insert(vec2, 0, new_int(0));

    darray_extend_at(vec1, 0, vec2);

    darray_clear(vec2);
    darray_append(vec2, new_int(5));
    darray_append(vec2, new_int(6));

    darray_extend(vec1, vec2);

    darray_foreach(vec1, (consumer) print_int);
    putchar('\n');

    del_darray(vec1);
    del_darray(vec2);

    return 0;
}
