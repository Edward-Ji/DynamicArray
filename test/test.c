#include <stdlib.h>

#include "../darray.h"
#include "minunit.h"

#define DARRAY_ASSERT_MATCH(arr, ...) do { \
    const int arr##_[] = {__VA_ARGS__}; \
    const size_t arr##_n = sizeof arr##_ / sizeof(int); \
    mu_assert(arr##_n == darray_len(arr), "array length mismatch"); \
    for (size_t i = 0; i < arr##_n; i++) { \
        int *intp = darray_get(arr, i); \
        mu_assert_int_eq(arr##_[i], *intp); \
    } \
} while (0)

#define DARRAY_APPEND_INTS(arr, ...) do { \
    const int arr##_[] = {__VA_ARGS__}; \
    const size_t arr##_n = sizeof arr##_ / sizeof(int); \
    for (size_t i = 0; i < arr##_n; i++) { \
        darray_append(arr, new_int(arr##_[i])); \
    } \
} while (0)

static darray *arr = NULL;

static long long sum = 0;

int *new_int(int x) {
    int *p = malloc(sizeof(int));
    *p = x;
    return p;
}

int int_cmp(const void *p1, const void *p2) {
    if (p1 == NULL || p2 == NULL) return 0;
    const int *intp1 = (const int *) p1;
    const int *intp2 = (const int *) p2;
    return (*intp1 > *intp2) - (*intp1 < *intp2);
}

void add_int_agg(const void *intp, void *resp) {
    if (resp == NULL || intp == NULL) return;
    *((long long *) resp) += *((const int *) intp);
}

void add_int_static(void *intp) {
    if (intp == NULL) {
        sum = 0;
        return;
    }
    sum += *((int *) intp);
}

void *int_cpy(const void *p) { return (void *) p; }

void *int_cpy_deep(const void *p) {
    int *cpy = malloc(sizeof(int));
    *cpy = *((int *) p);
    return cpy;
}

MU_TEST(test_new_int) {
    int *p = new_int(42);
    mu_assert_int_eq(42, *p);
    free(p);
}

MU_TEST(test_int_cmp1) {
    int x = 0, y = 1;
    mu_check(int_cmp(&x, &y) < 0);
}

MU_TEST(test_int_cmp2) {
    int x = 0, y = 0;
    mu_check(int_cmp(&x, &y) == 0);
}

MU_TEST(test_int_cmp3) {
    int x = 1, y = 0;
    mu_check(int_cmp(&x, &y) > 0);
}

MU_TEST(test_add_int_agg) {
    int x = 42, y = 42;
    long long res = 0;
    add_int_agg(&x, &res);
    add_int_agg(&y, &res);
    mu_check(84 == res);
}

MU_TEST(test_add_int_static) {
    int x = 42, y = 42;
    sum = 0;
    add_int_static(&x);
    add_int_static(&y);
    mu_check(84 == sum);
}

MU_TEST(test_int_cpy) {
    int x = 42;
    int *p = int_cpy(&x);
    mu_assert_int_eq(x, *p);
    mu_assert(&x == p, "shallow copy should have the same address");
}

MU_TEST(test_int_cpy_deep) {
    int x = 42;
    int *p = int_cpy_deep(&x);
    mu_assert_int_eq(x, *p);
    mu_assert(&x != p, "deep copy should not have the same address");
    free(p);
}

MU_TEST_SUITE(int_test_suite) {
    MU_RUN_TEST(test_new_int);
    MU_RUN_TEST(test_int_cmp1);
    MU_RUN_TEST(test_int_cmp2);
    MU_RUN_TEST(test_int_cmp3);
    MU_RUN_TEST(test_add_int_agg);
    MU_RUN_TEST(test_add_int_static);
    MU_RUN_TEST(test_int_cpy);
    MU_RUN_TEST(test_int_cpy_deep);
}

void darray_test_setup() {
    arr = new_darray(free);
    for (int i = 0; i < 5; i++) {
        darray_append(arr, new_int(i));
    }
}

void darray_test_teardown() {
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
    add_int_static(NULL);
    darray_foreach(arr, add_int_static);
    mu_check(sum == 0 + 1 + 2 + 3 + 4);
}

MU_TEST(test_darray_aggregate) {
    long long res = 0;
    darray_aggregate(arr, &res, add_int_agg);
    mu_check(res == 0 + 1 + 2 + 3 + 4);
}

MU_TEST(test_darray_pop1) {
    darray_pop(arr, 0);
    DARRAY_ASSERT_MATCH(arr, 1, 2, 3, 4);
}

MU_TEST(test_darray_pop2) {
    darray_pop(arr, 2);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 3, 4);
}

MU_TEST(test_darray_pop3) {
    darray_pop(arr, 4);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 2, 3);
}

MU_TEST(test_darray_pop_range1) {
    darray_pop_range(arr, 0, 5);
    DARRAY_ASSERT_MATCH(arr);
}

MU_TEST(test_darray_pop_range2) {
    darray_pop_range(arr, 1, 5);
    DARRAY_ASSERT_MATCH(arr, 0);
}

MU_TEST(test_darray_pop_range3) {
    darray_pop_range(arr, 0, 4);
    DARRAY_ASSERT_MATCH(arr, 4);
}

MU_TEST(test_darray_pop_range4) {
    darray_pop_range(arr, 1, 4);
    DARRAY_ASSERT_MATCH(arr, 0, 4);
}

MU_TEST(test_darray_insert1) {
    darray_insert(arr, 0, new_int(-1));
    DARRAY_ASSERT_MATCH(arr, -1, 0, 1, 2, 3, 4);
}

MU_TEST(test_darray_insert2) {
    darray_insert(arr, 2, new_int(-1));
    DARRAY_ASSERT_MATCH(arr, 0, 1, -1, 2, 3, 4);
}

MU_TEST(test_darray_insert3) {
    darray_insert(arr, 5, new_int(-1));
    DARRAY_ASSERT_MATCH(arr, 0, 1, 2, 3, 4, -1);
}

MU_TEST(test_darray_search1) {
    int val = 0;
    size_t idx = -1;
    mu_assert_int_eq(1, darray_search(arr, &val, int_cmp, &idx));
    mu_check(0 == idx);
}

MU_TEST(test_darray_search2) {
    int val = 2;
    size_t idx = -1;
    mu_assert_int_eq(1, darray_search(arr, &val, int_cmp, &idx));
    mu_check(2 == idx);
}

MU_TEST(test_darray_search3) {
    int val = 4;
    size_t idx = -1;
    mu_assert_int_eq(1, darray_search(arr, &val, int_cmp, &idx));
    mu_check(4 == idx);
}

MU_TEST(test_darray_extend1) {
    darray *arr2 = new_darray(NULL);
    darray_extend(arr, arr2);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 2, 3, 4);
    del_darray(arr2);
}

MU_TEST(test_darray_extend2) {
    darray *arr2 = new_darray(NULL);
    darray_append(arr2, new_int(5));
    darray_extend(arr, arr2);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 2, 3, 4, 5);
    del_darray(arr2);
}

MU_TEST(test_darray_extend3) {
    darray *arr2 = new_darray(NULL);
    DARRAY_APPEND_INTS(arr2, 5, 6);
    darray_extend(arr, arr2);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 2, 3, 4, 5, 6);
    del_darray(arr2);
}

MU_TEST(test_darray_extend_at1) {
    darray *arr2 = new_darray(NULL);
    DARRAY_APPEND_INTS(arr2, -2, -1);
    darray_extend_at(arr, 0, arr2);
    DARRAY_ASSERT_MATCH(arr, -2, -1, 0, 1, 2, 3, 4);
    del_darray(arr2);
}

MU_TEST(test_darray_extend_at2) {
    darray *arr2 = new_darray(NULL);
    darray_append(arr2, new_int(42));
    darray_extend_at(arr, 2, arr2);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 42, 2, 3, 4);
    del_darray(arr2);
}

MU_TEST(test_darray_extend_at3) {
    darray *arr2 = new_darray(NULL);
    DARRAY_APPEND_INTS(arr2, 5, 6);
    darray_extend_at(arr, 5, arr2);
    DARRAY_ASSERT_MATCH(arr, 0, 1, 2, 3, 4, 5, 6);
    del_darray(arr2);
}

MU_TEST(test_darray_reverse) {
    darray_reverse(arr);
    DARRAY_ASSERT_MATCH(arr, 4, 3, 2, 1, 0);
}

MU_TEST(test_darray_unique1) {
    darray *arr2 = new_darray(free);
    DARRAY_APPEND_INTS(arr2, 0, 0, 0, 1);
    darray_unique(arr2, int_cmp);
    DARRAY_ASSERT_MATCH(arr2, 0, 1);
    del_darray(arr2);
}

MU_TEST(test_darray_unique2) {
    darray *arr2 = new_darray(free);
    DARRAY_APPEND_INTS(arr2, 0, 1, 1, 1);
    darray_unique(arr2, int_cmp);
    DARRAY_ASSERT_MATCH(arr2, 0, 1);
    del_darray(arr2);
}

MU_TEST(test_darray_unique3) {
    darray *arr2 = new_darray(free);
    DARRAY_APPEND_INTS(arr2, 0, 1, 1, 0);
    darray_unique(arr2, int_cmp);
    DARRAY_ASSERT_MATCH(arr2, 0, 1, 0);
    del_darray(arr2);
}

MU_TEST(test_darray_sort) {
    darray *arr2 = new_darray(free);
    DARRAY_APPEND_INTS(arr2, 0, 1, 1, 0);
    darray_unique(arr2, int_cmp);
    DARRAY_ASSERT_MATCH(arr2, 0, 1, 0);
    del_darray(arr2);
}

MU_TEST(test_darray_clone1) {
    darray *arr2 = darray_clone(arr, int_cpy);
    darray_set_item_free(arr2, NULL);
    DARRAY_ASSERT_MATCH(arr2, 0, 1, 2, 3, 4);
    del_darray(arr2);
}

MU_TEST(test_darray_clone2) {
    darray *arr2 = darray_clone(arr, int_cpy_deep);
    DARRAY_ASSERT_MATCH(arr2, 0, 1, 2, 3, 4);
    del_darray(arr2);
}

MU_TEST(test_darray_clear) {
    darray_clear(arr);
    DARRAY_ASSERT_MATCH(arr);
}

MU_TEST_SUITE(darray_test_suite) {
    MU_SUITE_CONFIGURE(&darray_test_setup, &darray_test_teardown);

    MU_RUN_TEST(test_darray_setup);
    MU_RUN_TEST(test_darray_len);
    MU_RUN_TEST(test_darray_foreach);
    MU_RUN_TEST(test_darray_aggregate);
    MU_RUN_TEST(test_darray_pop1);
    MU_RUN_TEST(test_darray_pop2);
    MU_RUN_TEST(test_darray_pop3);
    MU_RUN_TEST(test_darray_pop_range1);
    MU_RUN_TEST(test_darray_pop_range2);
    MU_RUN_TEST(test_darray_pop_range3);
    MU_RUN_TEST(test_darray_pop_range4);
    MU_RUN_TEST(test_darray_insert1);
    MU_RUN_TEST(test_darray_insert2);
    MU_RUN_TEST(test_darray_insert3);
    MU_RUN_TEST(test_darray_search1);
    MU_RUN_TEST(test_darray_search2);
    MU_RUN_TEST(test_darray_search3);
    MU_RUN_TEST(test_darray_extend1);
    MU_RUN_TEST(test_darray_extend2);
    MU_RUN_TEST(test_darray_extend3);
    MU_RUN_TEST(test_darray_extend_at1);
    MU_RUN_TEST(test_darray_extend_at2);
    MU_RUN_TEST(test_darray_extend_at3);
    MU_RUN_TEST(test_darray_reverse);
    MU_RUN_TEST(test_darray_unique1);
    MU_RUN_TEST(test_darray_unique2);
    MU_RUN_TEST(test_darray_unique3);
    MU_RUN_TEST(test_darray_sort);
    MU_RUN_TEST(test_darray_clone1);
    MU_RUN_TEST(test_darray_clone2);
    MU_RUN_TEST(test_darray_clear);
}

int main() {
    MU_RUN_SUITE(int_test_suite);
    MU_RUN_SUITE(darray_test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
