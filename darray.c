/*!
\file darray.c
\author Edward Ji
\date 21 Jun 2022
\brief The source code of dynamic array of void pointers.

\warning Note that some types and functions have no declaration or incomplete
definition in the header file. The documentation in this source file is targeted
to maintainers.
*/

#include <stdlib.h>
#include <string.h>

#include "darray.h"

//! Represents a dynamic array structure.
struct darray {
    /*! Points to an allocated array of pointers to items. */
    void **item_ptr_arr;
    /*! Points to a function that frees an item in the array. */
    consumer item_free;
    /*! The number of items stored in the array. */
    size_t len;
    /*! The current capacity of the array. */
    size_t cap;
};

const size_t sizeof_darray = sizeof(darray);

darray_error darray_errno;

darray *new_darray(consumer item_free) {
    darray *array = malloc(sizeof(darray));
    if (array != NULL) {
        array->item_free = item_free;
        array->len = 0;
        array->cap = 1;
        array->item_ptr_arr = malloc(sizeof(void *) * array->cap);
        if (array->item_ptr_arr == NULL) {
            darray_errno = DARRAY_EALLOC;
            return NULL;
        }
    }

    return array;
}

int darray_set_item_free(darray *array, consumer item_free) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    array->item_free = item_free;

    return 1;
}

//! Changes the capacity of the dynamic array.
/*!
\param len The expected number of items stored in the array.
\returns The updated capacity of the array, 0 otherwise.
*/
static int darray_resize(darray *array, size_t len) {
    void **item_ptr_arr = array->item_ptr_arr;
    size_t cap = array->cap;

    if (len > cap) {
        while (len > cap) {
            cap *= 2;
        }
    } else {
        while (len < cap / 2 && cap > 1) {
            cap /= 2;
        }
    }
    if (cap != array->cap) {
        item_ptr_arr = realloc(item_ptr_arr, sizeof(void *) * cap);
        if (item_ptr_arr == NULL) {
            darray_errno = DARRAY_EALLOC;
            return 0;
        }
        array->item_ptr_arr = item_ptr_arr;
        array->cap = cap;
    }

    return array->cap != 0;
}

size_t darray_len(darray *array) {
    if (array == NULL) {
        return 0;
    }
    return array->len;
}

int darray_foreach(darray *array, consumer fp) {
    if (array == NULL || fp == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    for (size_t i = 0; i < array->len; i++) {
        fp(array->item_ptr_arr[i]);
    }

    return 1;
}

int darray_aggregate(darray *array, void *resp, aggregate fp) {
    if (array == NULL || fp == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    for (size_t i = 0; i < array->len; i++) {
        fp(array->item_ptr_arr[i], resp);
    }

    return 1;
}

int darray_append(darray *array, void *item_ptr) {
    if (array == NULL || item_ptr == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    if (!darray_resize(array, array->len + 1)) {
        return 0;
    }

    array->item_ptr_arr[array->len] = item_ptr;

    array->len++;

    return 1;
}

void *darray_get(darray *array, size_t index) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return NULL;
    }
    if (index >= array->len) {
        darray_errno = DARRAY_EINDEX;
        return NULL;
    }

    return array->item_ptr_arr[index];
}

int darray_pop(darray *array, size_t index) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }
    if (index >= array->len) {
        darray_errno = DARRAY_EINDEX;
        return 0;
    }

    if (array->item_free != NULL) {
        array->item_free(array->item_ptr_arr[index]);
    }
    memmove(array->item_ptr_arr + index,
            array->item_ptr_arr + index + 1,
            sizeof(void *) * (array->len - index - 1));
    if (!darray_resize(array, array->len - 1)) {
        return 0;
    }

    array->len--;

    return 1;
}

int darray_pop_range(darray *array, size_t start, size_t end) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }
    if (start > end || end > array->len) {
        darray_errno = DARRAY_EINDEX;
        return 0;
    }

    if (array->item_free != NULL) {
        for (size_t i = start; i < end; i++) {
            array->item_free(array->item_ptr_arr[i]);
        }
    }
    memmove(array->item_ptr_arr + start,
            array->item_ptr_arr + end,
            sizeof(void *) * (array->len - end));
    if (!darray_resize(array, array->len - (end - start))) {
        return 0;
    }

    array->len -= end - start;

    return 1;
}

int darray_insert(darray *array, size_t index, void *item_ptr) {
    if (array == NULL || item_ptr == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }
    if (index > array->len) {
        darray_errno = DARRAY_EINDEX;
        return 0;
    }

    if (!darray_resize(array, array->len + 1)) {
        return 0;
    }
    memmove(array->item_ptr_arr + index + 1,
            array->item_ptr_arr + index,
            sizeof(void *) * (array->len - index));

    array->item_ptr_arr[index] = item_ptr;

    array->len++;

    return 1;
}

int darray_search(
        darray *array, void *item_ptr, comparator fp, size_t *idx_ptr) {
    if (array == NULL || item_ptr == NULL || fp == NULL || idx_ptr == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    for (size_t i = 0; i < array->len; i++) {
        if (fp(array->item_ptr_arr[i], item_ptr) == 0) {
            *idx_ptr = i;
            return 1;
        }
    }

    darray_errno = DARRAY_ENOTIN;
    return 0;
}

int darray_extend_at(darray *array1, size_t index, darray *array2) {
    if (array1 == NULL || array2 == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }
    if (index > array1->len) {
        darray_errno = DARRAY_EINDEX;
        return 0;
    }

    if (!darray_resize(array1, array1->len + array2->len)) {
        return 0;
    }
    memmove(array1->item_ptr_arr + index + array2->len,
            array1->item_ptr_arr + index,
            sizeof(void *) * (array1->len - index));

    for (size_t i = 0; i < array2->len; i++) {
        array1->item_ptr_arr[index + i] = darray_get(array2, i);
    }

    array1->len += array2->len;

    return 1;
}

int darray_extend(darray *array1, darray *array2) {
    if (array1 == NULL || array2 == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    if (!darray_resize(array1, array1->len + array2->len)) {
        return 0;
    }

    for (size_t i = 0; i < array2->len; i++) {
        array1->item_ptr_arr[array1->len + i] = darray_get(array2, i);
    }

    array1->len += array2->len;

    return 1;
}

int darray_reverse(darray *array) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    for (size_t i = 0; i < array->len / 2; i++) {
        void *temp = array->item_ptr_arr[i];
        array->item_ptr_arr[i] = array->item_ptr_arr[array->len - i - 1];
        array->item_ptr_arr[array->len - i - 1] = temp;
    }

    return 1;
}

int darray_unique(darray *array, comparator fp) {
    if (array == NULL || fp == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    void **item_ptr_arr = array->item_ptr_arr;
    size_t m = 1;
    size_t i = 1;
    for (; i < array->len; i++) {
        if (fp(item_ptr_arr[i-1], item_ptr_arr[i]) != 0) {
            if (!darray_pop_range(array, m, i)) {
                return 0;
            }
            i -= i - m;
            m = i + 1;
        }
    }
    if (!darray_pop_range(array, m, i)) {
        return 0;
    }

    return 1;
}

static void swap_voidp(void **pp1, void **pp2) {
    void *temp = *pp1;
    *pp1 = *pp2;
    *pp2 = temp;
}

static ssize_t partition(void **item_ptr_arr,
                         ssize_t low, ssize_t high, comparator cmp) {
    void *pivot = item_ptr_arr[high];
    ssize_t i = low - 1;

    for (ssize_t j = low; j < high; j++) {
        if (cmp(item_ptr_arr[j], pivot) < 0) {
            i++;
            swap_voidp(item_ptr_arr + i, item_ptr_arr + j);
        }
    }
    swap_voidp(item_ptr_arr + (i + 1), item_ptr_arr + high);
    return i + 1;
}

void darray_qsort(void **item_ptr_arr, ssize_t low, ssize_t high, comparator cmp) {
    if (low < high) {
        ssize_t pivot_i = partition(item_ptr_arr, low, high, cmp);

        darray_qsort(item_ptr_arr, low, pivot_i - 1, cmp);
        darray_qsort(item_ptr_arr, pivot_i + 1, high, cmp);
    }
}

int darray_sort(darray *array, comparator fp) {
    if (array == NULL || fp == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    if (array->len > 0) {
        darray_qsort(array->item_ptr_arr, 0, array->len - 1, fp);
    }

    return 1;
}

darray *darray_clone(darray *array, unary fp) {
    if (array == NULL || fp == NULL) {
        darray_errno = DARRAY_ENULLS;
        return NULL;
    }

    darray *clone = (darray *) malloc(sizeof(darray));

    memcpy(clone, array, sizeof(darray));

    clone->item_ptr_arr = (void **) malloc(sizeof(void *) * clone->cap);
    for (size_t i = 0; i < clone->len; i++) {
        clone->item_ptr_arr[i] = fp(array->item_ptr_arr[i]);
    }

    return clone;
}

int darray_clear(darray *array) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    for (size_t i = 0; i < array->len; i++) {
        if (array->item_free != NULL) {
            array->item_free(array->item_ptr_arr[i]);
        }
    }
    array->len = 0;

    return 1;
}

int del_darray(darray *array) {
    if (array == NULL) {
        darray_errno = DARRAY_ENULLS;
        return 0;
    }

    darray_clear(array);
    free(array->item_ptr_arr);
    free(array);

    return 1;
}

static const char *const darray_strerr_list[] = {
    [DARRAY_EALLOC] = "fail to allocate memory",
    [DARRAY_ENULLS] = "invalid NULL argument",
    [DARRAY_EINDEX] = "invalid index",
    [DARRAY_ENOTIN] = "item does not exist"
};

const char *darray_strerr() {
    if (darray_errno == DARRAY_ERESET) {
        return NULL;
    }

    const char *str = darray_strerr_list[darray_errno];
    if (str == NULL) {
        str = "unknown error";
    }

    darray_errno = DARRAY_ERESET;
    return str;
}
