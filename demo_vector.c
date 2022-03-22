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

int compare_int(const int *p1, const int *p2) {
    if (*p1 < *p2) {
        return -1;
    } else if (*p1 == *p2) {
        return 0;
    } else {
        return 1;
    }
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
    darray_append(vec2, new_int(5));
    darray_append(vec2, new_int(6));
    darray_append(vec2, new_int(6));
    darray_append(vec2, new_int(6));

    puts("vector 2 before unique");
    darray_foreach(vec2, (consumer) print_int);
    puts("\n");

    darray_set_item_free(vec2, free);
    darray_unique(vec2, (comparator) compare_int);
    darray_set_item_free(vec2, NULL);

    puts("vector 2 after unique");
    darray_foreach(vec2, (consumer) print_int);
    puts("\n");

    darray_extend(vec1, vec2);

    puts("vector 1 after extending vector 2");
    darray_foreach(vec1, (consumer) print_int);
    puts("\n");

    darray_reverse(vec1);

    puts("vector 1 after reversing");
    darray_foreach(vec1, (consumer) print_int);
    puts("\n");

    darray_sort(vec1, (comparator) compare_int);

    puts("vector 1 after sorting");
    darray_foreach(vec1, (consumer) print_int);
    puts("\n");

    darray_pop_range(vec1, 3, 7);

    puts("vector 1 after poping range [3, 7)");
    darray_foreach(vec1, (consumer) print_int);
    puts("\n");

    del_darray(vec1);
    del_darray(vec2);

    return 0;
}
