#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

struct darray {
    void **itempp;
    consumer item_free;
    size_t len;
    size_t cap;
};

const size_t sizeof_darray = sizeof(darray);

darray *new_darray(consumer item_free) {
    darray *arrp = malloc(sizeof(darray));
    if (arrp != NULL) {
        arrp->item_free = item_free;
        arrp->len = 0;
        arrp->cap = 1;
        arrp->itempp = malloc(sizeof(void *) * arrp->cap);
        if (arrp->itempp == NULL) {
            return NULL;
        }
    }

    return arrp;
}

int _darray_resize(darray *arrp, size_t len) {
    if (arrp == NULL) {
        return 0;
    }

    void **itempp = arrp->itempp;
    size_t cap = arrp->cap;

    if (len > cap) {
        while (len > cap) {
            cap *= 2;
        }
    } else {
        while (len <= cap / 2 && cap > 1) {
            cap /= 2;
        }
    }
    if (cap != arrp->cap) {
        itempp = realloc(itempp, sizeof(void *) * cap);
        if (itempp == NULL) {
            return 0;
        }
        arrp->itempp = itempp;
        arrp->cap = cap;
    }

    return arrp->cap;
}

size_t darray_len(darray *arrp) {
    return arrp->len;
}

int darray_foreach(darray *arrp, consumer fp) {
    if (arrp == NULL || fp == NULL) {
        return 0;
    }

    for (size_t i = 0; i < arrp->len; i++) {
        fp(arrp->itempp[i]);
    }

    return 1;
}

void darray_aggregate(darray *arrp, void *resp, aggregate fp) {
    if (arrp == NULL || fp == NULL) {
        return;
    }

    for (size_t i = 0; i < arrp->len; i++) {
        fp(arrp->itempp[i], resp);
    }
}

int darray_append(darray *arrp, void *itemp) {
    if (arrp == NULL || itemp == NULL) {
        return 0;
    }

    if (!_darray_resize(arrp, arrp->len + 1)) {
        return 0;
    }

    arrp->itempp[arrp->len] = itemp;

    arrp->len++;

    return 1;
}

void *darray_get(darray *arrp, size_t index) {
    if (arrp == NULL || index >= arrp->len) {
        return NULL;
    }

    return arrp->itempp[index];
}

int darray_pop(darray *arrp, size_t index) {
    if (arrp == NULL || index >= arrp->len) {
        return 0;
    }

    if (arrp->item_free != NULL) {
        arrp->item_free(arrp->itempp[index]);
    }
    memmove(arrp->itempp + index,
            arrp->itempp + index + 1,
            sizeof(void *) * (arrp->len - index));
    if (!_darray_resize(arrp, arrp->len - 1)) {
        return 0;
    }

    arrp->len--;

    return 1;
}

int darray_insert(darray *arrp, size_t index, void *itemp) {
    if (arrp == NULL || index > arrp->len || itemp == NULL) {
        return 0;
    }

    if (!_darray_resize(arrp, arrp->len + 1)) {
        return 0;
    }
    memmove(arrp->itempp + index + 1,
            arrp->itempp + index,
            sizeof(void *) * (arrp->len - index));

    arrp->itempp[index] = itemp;

    arrp->len++;

    return 1;
}

int darray_search(darray *arrp, void *itemp, comparator fp, size_t *indexp) {
    if (arrp == NULL || itemp == NULL || fp == NULL || indexp == NULL) {
        return 0;
    }

    for (size_t i = 0; i < arrp->len; i++) {
        if (fp(arrp->itempp[i], itemp) == 0) {
            *indexp = i;
            return 1;
        }
    }

    return 0;
}

int darray_extend_at(darray *arrp1, size_t index, darray *arrp2) {
    if (arrp1 == NULL || index > arrp1->len || arrp2 == NULL) {
        return 0;
    }

    if (!_darray_resize(arrp1, arrp1->len + arrp2->len)) {
        return 0;
    }
    memmove(arrp1->itempp + index + arrp2->len,
            arrp1->itempp + index,
            sizeof(void *) * (arrp1->len - index));

    for (size_t i = 0; i < arrp2->len; i++) {
        arrp1->itempp[index + i] = darray_get(arrp2, i);
    }

    arrp1->len += arrp2->len;

    return 1;
}

int darray_extend(darray *arrp1, darray *arrp2) {
    if (arrp1 == NULL || arrp2 == NULL) {
        return 0;
    }

    if (!_darray_resize(arrp1, arrp1->len + arrp2->len)) {
        return 0;
    }

    for (size_t i = 0; i < arrp2->len; i++) {
        arrp1->itempp[arrp1->len + i] = darray_get(arrp2, i);
    }

    arrp1->len += arrp2->len;

    return 1;
}

int darray_clear(darray *arrp) {
    if (arrp == NULL) {
        return 0;
    }

    for (size_t i = 0; i < arrp->len; i++) {
        if (arrp->item_free != NULL) {
            arrp->item_free(arrp->itempp[i]);
        }
    }
    arrp->len = 0;

    return 1;
}

void del_darray(darray *arrp) {
    if (arrp == NULL) {
        return;
    }

    darray_clear(arrp);
    free(arrp->itempp);
    free(arrp);
}
