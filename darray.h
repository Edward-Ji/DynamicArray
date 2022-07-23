/*!
\file darray.h
\author Edward Ji
\date 21 Jun 2022
\brief Dynamic array of void pointers with automatic memory free.
*/
#ifndef DARRAY_H
#define DARRAY_H

#include <stddef.h>
#include <sys/types.h>

//! The consumer function pointer type definition.
/*!
A function of this type should take in a pointer to some object and perform
some operation on that object. The function should not return anything.

\param itemp Pointer to an object in the array.

\see Typically used with `darray_foreach`.

An example of a consumer function pointer is a function that prints the
contents at some given pointer:
```
void print_int(void *p) {
    printf("%d", *((int *) p));
}
```
*/
typedef void (*consumer)(void *itemp);

//! The aggregate function pointer type definition.
/*!
A function of this type should take in a pointer to some object and modifies
the result given by the second pointer. The function should not return
anything or modify the first object.

\param itemp A pointer to some object.
\param resp A pointer to the result.

\see Typically used with `darray_aggregate`.

An example of an aggregate function pointer is a function that takes in a
pointer to an integer and adds that integer to a result object at the second
pointer.
```
void int_sum(void *itemp, void *resp) {
    *((long long *) resp) += *((int *) itemp);
}
```
*/
typedef void (*aggregate)(const void *itemp, void *resp);

//! The comparator function pointer type definition.
/**
A function of this type should take in two pointers to objects and compares
them. It should modify neither object nor should it return anything.

\param itemp1 A pointer to some object.
\param itemp2 A pointer to some object to compare against.
\returns
- A negative number if the first object is smaller;
- Zero if they are equal; or
- A positive number if the first object is bigger.

An example of a comparator function pointer is a function that takes in two
pointers to integers and compares them as per the rules above:
```
void int_cmp(void *p1, void *p2) {
    int x = *((int *) p1);
    int y = *((int *) p2);
    return (x > y) - (x < y);
}
```

\note The two pointers do not necessarily point to the same type.  For example,
given a structure with a field `name` of type `char[]`. You can write a
comparator that compares this field against some string. The `has_name` function
is a valid comparator:
```
struct person {
    char name[42];
    int age;
}

int has_name(const struct person *person, const char *name) {
    return strcmp(person->name, name);
}
```
This also how you would search for a person with a given name using
`darray_search`.
*/
typedef int (*comparator)(const void *itemp1, const void *itemp2);

//! The unary function pointer type definition.
/*!
A function of this type should take in a pointer to some object and return a
pointer to that or some other object (e.g. a copy of the original object). It
should not modify the original object.

\param itemp A pointer to some object.
\returns A pointer to some other object, or the given object.

An example of a unary function pointer is a function that takes in the pointer
and returns it. This is useful for creating a shallow copy of an array with
`darray_clone`.
```
void *identity(const void *p) { return p; }
```
*/
typedef void *(*unary)(const void *itemp);

//! Represents a dynamic array.
typedef struct darray darray;

//! The size of the dynamic array structure.
/*!
Use this instead of `sizeof(darray)` because the dynamic array structure
definition is incomplete in this header.
*/
extern const size_t sizeof_darray;

//! Creates a new dynamic array of void pointers.
/*!
The function allocates a new dynamic array of generic pointers and returns it.
It accepts a consumer function pointer that deallocates an item and its
components. The free function can be `NULL` which means that the items does not
need to be freed. This could be useful for creating a shallow copy (view) of the
original array.

\param item_free A pointer to a function that frees an item.
\returns A new dynamic array object.
\see To deallocate the dynamic array, use `del_darray`.
*/
darray *new_darray(consumer item_free);

//!Sets the free function.
/*!
This function sets the function pointer that frees the item if the item pointer
is removed from the array. You should rarely need to use this function, except
for temporarily disabling the free behaviour of other dynamic array functions.

\param arrp A pointer to a dynamic array.
\param item_free A pointer to a function that frees an item.
\return 1 if successful, 0 otherwise.

\note You can temporarily disable automatic deallocation of array items by
setting `item_free` to `NULL` and back.
*/
int darray_set_item_free(darray *arrp, consumer item_free);

//! Getter for the length of the array.
/*!
This function returns the number of items in a given array.

\param arrp A pointer to a dynamic array.
\returns The number of items in a given array.
*/
ssize_t darray_len(darray *arrp);

//! Calls each item in the array with a given function.
/*!
This function calls the given function with every object in the array
sequentially.

\param arrp A pointer to a dynamic array.
\param fp A pointer to a consumer function.
\returns 1 if successful, 0 otherwise.
*/
int darray_foreach(darray *arrp, consumer fp);

//! Aggregates all items into a single result.
/*!
This function calls the aggregation function with every item in the array as the
first argument, and the result pointer as the second argument.

\param arrp A pointer to a dynamic array.
\param resp A pointer to the result object.
\param fp A pointer to an aggregate function.
\see How to write an `aggregate` function.
*/
void darray_aggregate(darray *arrp, void *resp, aggregate fp);

//! Appends an item to the array.
/*!
This function appends an item to the end of an array.

\param arrp A pointer to a dynamic array.
\param itemp A pointer to an item to be appended.
\returns 1 if successful, 0 otherwise.
*/
int darray_append(darray *arrp, void *itemp);

//! Gets an item in an array using index.
/*!
This function attempts to return the item at a given index in the array.

\param arrp A pointer to a dynamic array.
\param index A valid index in the array.
\returns The item at the given index in the array if successful, `NULL`
otherwise.

\note The function is unsuccessful if the pointer to an array is `NULL` or the
index is out of range.
*/
void *darray_get(darray *arrp, size_t index);

//! Pops an item at a given index.
/*!
This function pops the item at a given index from an array.

\param arrp A pointer to a dynamic array.
\param index A valid index in the array.
\returns 1 if successful, 0 otherwise.

\warning The free function associated with the array is called if it's not
`NULL`. If the function properly deallocates the item, accessing the popped
object may be undefined behavior.
```
arr = new_darray(free);
int *p = malloc(sizeof(int));
*p = 42;
darray_append(arr, p);
darray_pop(0); // `free(p)` called
*p = 0;        // undefined behavior
```
*/
int darray_pop(darray *arrp, size_t index);

//! Pops items at a given index range.
/*!
This function pops the items at a given index range from the array. This
includes the item at the starting index, the items between the starting index
and the ending index, but **not** the item at the end index.

\param arrp A pointer to a dynamic array.
\param start An index from which to start popping (inclusive).
\param end An index at which to stop popping (exclusive).
\returns 1 if successful, 0 otherwise.

\warning The free function associated with the array is called for each item if
the function is not `NULL`. If the function properly deallocates the items,
accessing any popped object may be undefined behavior.
*/
int darray_pop_range(darray *arrp, size_t start, size_t end);

//! Inserts an item at a given index.
/*!
This function inserts an item at an given index in an array.

\param arrp A pointer to a dynamic array.
\param index A valid index to insert at.
\param itemp A pointer to an item to be appended.
\returns 1 if successful, 0 otherwise.
*/
int darray_insert(darray *arrp, size_t index, void *itemp);

//! Searches for an item in an array that compares equal to anther object.
/*!
Searches for an item in the array using a given comparator and stores its
index in the index pointer. The comparator is called with an array item as the
first argument, and the item to compare against as the second argument.

\param arrp A pointer to a dynamic array.
\param itemp An object to compare against.
\param fp A pointer to a function that compares array item against the object.
\param indexp A pointer to store the index of found item.
\returns 1 if there is a match, or 0 otherwise.

\see How to write a `comparator`.

For example, to find the index of `42` in some integer array:
```
int target = 42;
size_t index;
if (!darray_search(array_pointer, &42, integer_comparator, &index)) {
    printf("42 not in array!");
    exit(1);
}
printf("42 at index %zu.", index);
```
*/
int darray_search(darray *arrp, void *itemp, comparator fp, size_t *indexp);

//! Extends another array to the end of a given array.
/*!
In the order of their index, append each item in the second array to the end of
the first one.

\param arrp1 A pointer to a dynamic array to extend to.
\param arrp2 A pointer to another dynamic array to extend from.
\return 1 if successful, 0 otherwise.
*/
int darray_extend(darray *arrp1, darray *arrp2);

//! Extends another array at a given index in a given array.
/*!
In the order of their index, insert each item in the second array at the given
index of the first one.

\param arrp1 A pointer to a dynamic array to extend to.
\param index A valid index in the first array to extend at.
\param arrp2 A pointer to another dynamic array to extend from.
\return 1 if successful, 0 otherwise.
*/
int darray_extend_at(darray *arrp1, size_t index, darray *arrp2);

//! Reverses a given array.
/*!
This function reverses a given array **in place**.

\param arrp A pointer to a dynamic array.
\returns 1 if successful, 0 otherwise.
*/
int darray_reverse(darray *arrp);

//! Filters out repeated adjacent items.
/*!
The second and succeeding copies of equal adjacent items are popped from the
array. The items are considered equal if they return `0` when passed into a
given comparator function. The function performs the above **in place**.

\param arrp A pointer to a dynamic array.
\param fp A pointer to a function that compares two items in the array.
\returns 1 if successful, 0 otherwise.

\note The behavior of this function mimics that of the Unix `uniq` utility.
*/
int darray_unique(darray *arrp, comparator fp);

//! Sorts a given array.
/*!
Sorts all items in the given array **in place** using a quick sort algorithm.

\param arrp A pointer to a dynamic array.
\param fp A pointer to a function that compares two items in the array.
\returns 1 if successful, 0 otherwise.

The sorting algorithm is adopted from [Quick Sort.c] by adwiteeya3 on GitHub.
[Quick Sort.c]: https://gist.github.com/adwiteeya3/f1797534506be672b591f465c3366643
*/
int darray_sort(darray *arrp, comparator fp);

//! Returns a clone of a given array.
/*!
This function calls the clone function on each item in the array and returns
them in a new array. The new array inherits the free function.

\param arrp A pointer to a dynamic array.
\param fp A pointer to a function that, given an item of the array, produces a
clone.
\returns A new allocated dynamic array.

\note Depending on the clone function provided, this function can produce a
shallow or deep copy of the given array. For example, given an array of integer,
```
void *shallow(void *p) { return p; }
void *deep(void *p) {
    int *cpy = malloc(sizeof(int));
    *cpy = *((int *) p);
    return cpy;
}

integer_array = new_darray(free);
// Omit: adding integer items to the array.
clone1 = darray_clone(integer_array, shallow); // creates a shallow copy
darray_set_item_free(clone1, NULL);            // usually required logically
clone2 = darray_clone(integer_array, deep);    // creates a deep copy
```
*/
darray *darray_clone(darray *arrp, unary fp);

//! Clears all items from a given array.
/*!
This function pops all items from the array and calls the free function on each
of them.

\param arrp A pointer to a dynamic array to clear.
\returns 1 if successful, 0 otherwise.
*/
int darray_clear(darray *arrp);

//! Deallocates a given array.
/*!
This function clears all items from a given array and deallocates the dynamic
array structure.

\param arrp A pointer to a dynamic array to deallocate.
*/
int del_darray(darray *arrp);

#endif
