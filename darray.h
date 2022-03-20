#ifndef DARRAY_H
#define DARRAY_H

#include <stddef.h>

typedef void (*consumer)(void *);
typedef int (*comparator)(void *, void *);

typedef struct darray darray;

struct darray {
    void **itempp;
    consumer item_free;
    size_t item_size;
    size_t len;
    size_t cap;
};

/*
 * Creates a new dynamic array with items of a generic size. The free function
 * pointer must be provided to free any allocated memory of the items.
 */
darray *new_darray(size_t item_size, consumer item_free);

/*
 * Call the given function on every object in the array sequentially.
 */
int darray_foreach(darray *arrp, consumer fp);

/*
 * Add a copy of the item at the end of the array.
 */
int darray_append(darray *arrp, void *itemp);

/*
 * Remove all items from the array.
 */
int darray_clear(darray *arrp);

/*
 * Remove all items and free the array's memory.
 */
void del_darray(darray *arrp);

#endif
