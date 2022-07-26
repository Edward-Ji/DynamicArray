# Dynamic Array

An implementation of a dynamic array of void pointers in C language.

## Abstract

By providing an item free function when creating the array, you need not worry
about deallocating the memory manually when removing items from the array, the
functions will handle it for you. Being dynamic, the array automatically resizes
if it has insufficient or excess capacity. The implementation also mimics a
functional programming approach by accepting function pointers to iteratively
operate on data.

[GitHub]: https://github.com/Edward-Ji/DynamicArray
