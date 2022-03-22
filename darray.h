#ifndef DARRAY_H
#define DARRAY_H

#include <stddef.h>

typedef void (*consumer)(void *);
typedef void (*aggregate)(void *, void *);
typedef int (*comparator)(void *, void *);

typedef struct darray darray;

extern const size_t sizeof_darray;

/*
 * Creates a new dynamic array with items of a generic size. The free function
 * pointer must be provided to free any allocated memory of the items.
 */
darray *new_darray(size_t item_size, consumer item_free);

/*
 * Returns the length of the given dynamic array.
 */
size_t darray_len(darray *arrp);

/*
 * Call the given function on every object in the array sequentially.
 */
int darray_foreach(darray *arrp, consumer fp);

/*
 * Call the aggregation function with every item in the array as the first
 * argument, and the result pointer as the seconds argument.
 */
void darray_aggregate(darray *arrp, void *resp, aggregate fp);

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
 * Search for the item in the array using the given camparator and stores its
 * index in the index pointer. The comparator is called with array items as the
 * first parameter, and should return 0 on equality and a
 * non-zero value otherwise. This function returns 1 if there os a match, or 0
 * otherwise.
 */
int darray_search(darray *arrp, void *itemp, comparator fp, size_t *indexp);

/*
 * Remove all items from the array.
 */
int darray_clear(darray *arrp);

/*
 * Remove all items and free the array's memory.
 */
void del_darray(darray *arrp);

#endif
