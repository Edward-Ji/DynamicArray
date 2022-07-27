/*!
\file vector.c
\author Edward Ji
\date 21 Jun 2022

\brief
A demonstration of the typical use cases of all dynamic array functions.

\stdout
```
new_darray(free)
	vec1 = [ 41 ]
	sum = 0
darray_append(vec1, new_int(1)) returns 1
	vec1 = [ 41 1 ]
darray_insert(vec1, 0, new_int(2)) returns 1
	vec1 = [ 2 41 1 ]
darray_foreach(vec1, int_inc) returns 1
	vec1 = [ 3 42 2 ]
darray_aggregate(vec1, &sum, int_add_agg) returns 1
	vec1 = [ 3 42 2 ]
	sum = 47
darray_search(vec1, &val, int_cmp, &idx) returns 1
	vec1 = [ 3 42 2 ]
	val = 42
	idx = 1
p1 = darray_get(vec1, 1)
	vec1 = [ 3 42 2 ]
	*p1 = 42
darray_extend(vec1, vec2) returns 1
	vec1 = [ 3 42 2 2 1 ]
	vec2 = [ 2 1 ]
darray_extend_at(vec1, 0, vec3) returns 1
	vec1 = [ 1 2 3 42 2 2 1 ]
	vec3 = [ 1 2 ]
darray_reverse(vec1) returns 1
	vec1 = [ 1 2 2 42 3 2 1 ]
darray_unique(vec1, int_cmp) returns 1
	vec1 = [ 1 2 42 3 2 1 ]
darray_sort(vec1, int_cmp) returns 1
	vec1 = [ 1 1 2 2 3 42 ]
vec4 = darray_clone(vec1, int_cpy_deep)
	vec1 = [ 1 1 2 2 3 42 ]
	vec4 = [ 1 1 2 2 3 42 ]
darray_clear(vec1) returns 1
	vec1 = [ ]
	vec4 = [ 1 1 2 2 3 42 ]
```
*/

#include <stdio.h>
#include <stdlib.h>

#include "../darray.h"

#define RUN_PRINT_EXPR(expr, arr) do {                                         \
    printf(#expr " returns %d\n\t" #arr " = ", (expr));                        \
    print_int_arr(arr);                                                        \
} while (0)

#define RUN_PRINT_STAT(stat, arr) do {                                         \
    stat;                                                                      \
    printf(#stat "\n\t" #arr " = ");                                           \
    print_int_arr(arr);                                                        \
} while (0)

int *new_int(int val) {
    int *p = (int *) malloc(sizeof(int));
    *p = val;

    return p;
}

void print_int(void *p) {
    printf("%d ", *((int *) p));
}

void int_inc(void *p) {
    (*((int *) p))++;
}

int int_cmp(const void *p1, const void *p2) {
    const int *ip1 = p1;
    const int *ip2 = p2;

    return (*ip1 > *ip2) - (*ip1 < *ip2);
}

void int_add_agg(const void *p1, void *p2) {
    *((long long *) p2) += *((int *) p1);
}

void *int_cpy_deep(const void *p) {
    int *cpy = (int *) malloc(sizeof(int));
    *cpy = *((int *)p);

    return cpy;
}

void print_int_arr(darray *arr) {
    printf("[ ");
    darray_foreach(arr, print_int);
    printf("]\n");
}

int main() {
    darray *vec1 = new_darray(free);
    darray *vec2 = new_darray(NULL);
    darray *vec3 = new_darray(NULL);
    darray *vec4;
    long long sum = 0;

    darray_append(vec1, new_int(41));
    darray_append(vec2, new_int(2));
    darray_append(vec2, new_int(1));
    darray_append(vec3, new_int(1));
    darray_append(vec3, new_int(2));

    printf("new_darray(free)\n");
    printf("\tvec1 = ");
    print_int_arr(vec1);
    printf("\tsum = %lld\n", sum);

    RUN_PRINT_EXPR(darray_append(vec1, new_int(1)), vec1);

    RUN_PRINT_EXPR(darray_insert(vec1, 0, new_int(2)), vec1);

    RUN_PRINT_EXPR(darray_foreach(vec1, int_inc), vec1);

    RUN_PRINT_EXPR(darray_aggregate(vec1, &sum, int_add_agg), vec1);
    printf("\tsum = %lld\n", sum);

    int val = 42;
    size_t idx;
    RUN_PRINT_EXPR(darray_search(vec1, &val, int_cmp, &idx), vec1);
    printf("\tval = %d\n", val);
    printf("\tidx = %zu\n", idx);

    int *p1;
    RUN_PRINT_STAT(p1 = darray_get(vec1, 1), vec1);
    printf("\t*p1 = %d\n", *p1);

    RUN_PRINT_EXPR(darray_extend(vec1, vec2), vec1);
    printf("\tvec2 = ");
    print_int_arr(vec2);

    RUN_PRINT_EXPR(darray_extend_at(vec1, 0, vec3), vec1);
    printf("\tvec3 = ");
    print_int_arr(vec3);

    RUN_PRINT_EXPR(darray_reverse(vec1), vec1);

    RUN_PRINT_EXPR(darray_unique(vec1, int_cmp), vec1);

    RUN_PRINT_EXPR(darray_sort(vec1, int_cmp), vec1);

    RUN_PRINT_STAT(vec4 = darray_clone(vec1, int_cpy_deep), vec1);
    printf("\tvec4 = ");
    print_int_arr(vec4);

    RUN_PRINT_EXPR(darray_clear(vec1), vec1);
    printf("\tvec4 = ");
    print_int_arr(vec4);

    del_darray(vec1);
    del_darray(vec2);
    del_darray(vec3);
    del_darray(vec4);

    return 0;
}
