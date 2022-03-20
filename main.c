#include <stdio.h>
#include <stdlib.h>

#include "darray.h"

#define N_ROWS 3
#define N_COLS 4

void print_int(int *p) {
    printf("%d ", *p);
}

void print_row(darray *arrp) {
    printf("[ ");
    darray_foreach(arrp, (consumer) print_int);
    printf("]\n");
}

int main() {
    int num;
    
    darray *matirxp = new_darray(sizeof(darray), (consumer) del_darray);
    darray *rowp;
    for (size_t i = 0; i < N_ROWS; i++) {
        rowp = new_darray(sizeof(int), NULL);
        for (size_t j = 0; j < N_COLS; j++) {
            num = i * N_COLS + j;
            darray_append(rowp, &num);
        }
        darray_append(matirxp, rowp);
    }
    
    darray_foreach(matirxp, (consumer) print_row);
    
    del_darray(matirxp);

    return 0;
}
