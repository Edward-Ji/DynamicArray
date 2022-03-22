#ifndef DARRAY_H
#define DARRAY_H

#include <stddef.h>

typedef void (*consumer)(void *);
typedef void (*aggregate)(const void *, void *);
typedef int (*comparator)(const void *, const void *);

typedef struct darray darray;

extern const size_t sizeof_darray;

/*
 * Creates a new dynamic array with items of a generic size. The free function
 * pointer must be provided to free any allocated memory of the items.
 */
darray *new_darray(consumer item_free);

/*
 * Sets the function pointer that frees the item if the item pointer is removed
 * from the array.
 */
int darray_set_item_free(darray *arrp, consumer item_free);

/*
 * Returns the length of the given dynamic array.
 */
size_t darray_len(darray *arrp);

/*
 * Calls the given function on every object in the array sequentially.
 */
int darray_foreach(darray *arrp, consumer fp);

/*
 * Calls the aggregation function with every item in the array as the first
 * argument, and the result pointer as the seconds argument.
 */
void darray_aggregate(darray *arrp, void *resp, aggregate fp);

/*
 * Adds a copy of the item at the end of the array.
 */
int darray_append(darray *arrp, void *itemp);

/*
 * Gets the item at a given index in the array.
 */
void *darray_get(darray *arrp, size_t index);

/*
 * Removes the item at a given index in the array.
 */
int darray_pop(darray *arrp, size_t index);

/*
 * Removes the items within a given index range in the array. The index range
 * includes all indices between start (inclusive) and end (exclusive).
 */
int darray_pop_range(darray *arrp, size_t start, size_t end);

/*
 * Inserts the item at a given index in the array.
 */
int darray_insert(darray *arrp, size_t index, void *itemp);

/*
 * Searches for the item in the array using the given comparator and stores its
 * index in the index pointer. The comparator is called with array items as the
 * first parameter, and should return 0 on equality and a non-zero value
 * otherwise. This function returns 1 if there is a match, or 0 otherwise.
 */
int darray_search(darray *arrp, void *itemp, comparator fp, size_t *indexp);

/*
 * Inserts all items of the second array at the given index in the first array
 * in the order they appear in the second array.
 */
int darray_extend_at(darray *arrp1, size_t index, darray *arrp2);

/*
 * Appends all items of the second array to the end of the first array.
 */
int darray_extend(darray *arrp1, darray *arrp2);

/*
 * Reverses all items of the given array in place.
 */
int darray_reverse(darray *arrp);

/*
 * Removes all adjacent unique items in the given array. The comparator is
 * called to compare each pair of adjacent items, and should only return 0 on
 * equality.
 */
int darray_unique(darray *arrp, comparator fp);

/*
 * Sorts all items in the given array in place using the quick sort algorithm.
 * The comparator should return a negative number if the first item is smaller,
 * 0 if the items are equal, and a positive number if the first item is greater.
 */
int darray_sort(darray *arrp, comparator fp);

/*
 * Removes all items from the array.
 */
int darray_clear(darray *arrp);

/*
 * Removes all items and free the array's memory.
 */
void del_darray(darray *arrp);

#endif
