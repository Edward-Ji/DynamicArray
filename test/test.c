#include <stdlib.h>

#include "../darray.h"
#include "minunit.h"

static darray *arr = NULL;

static long long sum = 0;

int *new_int(int x) {
    int *p = malloc(sizeof(int));
    *p = x;
    return p;
}

int cmp_int(int *p1, int *p2) {
    return (p1 < p2) - (p2 < p1);
}

void add_int_agg(const void *intp, void *resp) {
    *((long long *) resp) += *((const int *) intp);
}

void add_int_static(void *intp) {
    sum += *((int *) intp);
}

void test_setup() {
    arr = new_darray(free);
    for (int i = 0; i < 5; i++) {
        darray_append(arr, new_int(i));
    }
}

void test_teardown() {
    del_darray(arr);
    arr = NULL;
}

MU_TEST(test_darray_setup) {
    mu_assert(arr != NULL, "fail to create new array");
    for (int i = 0; i < 5; i++) {
        int *p = darray_get(arr, i);
        if (p == NULL) mu_fail("fail to get item from array");
        mu_assert_int_eq(i, *p);
    }
}

MU_TEST(test_darray_len) {
    mu_assert_int_eq(5, darray_len(arr));
}

MU_TEST(test_darray_foreach) {
    darray_foreach(arr, add_int_static);
    mu_check(sum == 0 + 1 + 2 + 3 + 4);
}

MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_darray_setup);
    MU_RUN_TEST(test_darray_len);
    MU_RUN_TEST(test_darray_foreach);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
