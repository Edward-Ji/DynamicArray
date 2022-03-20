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
 * Get the item at a given index in the array.
 */
void *darray_get(darray *arrp, size_t index);

/*
 * Remove the item at a given index in the array.
 */
int darray_pop(darray *arrp, size_t index);

/*
 * Insert the item at a given index in the array.
 */
int darray_insert(darray *arrp, size_t index, void *itemp);

/*
 * Search for the item in the array using the given camparator and return its
 * index. The comparator should return 0 on equality and a non-zero value
 * otherwise. If the item is not found, it returns a negative number.
 */
ssize_t array_index(darray *arrp, void *itemp, comparator fp);

/*
 * Remove all items from the array.
 */
int darray_clear(darray *arrp);

/*
 * Remove all items and free the array's memory.
 */
void del_darray(darray *arrp);

#endif
