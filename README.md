# Dynamic Array

[![Automated Testing](https://github.com/Edward-Ji/DynamicArray/actions/workflows/test.yml/badge.svg)](https://github.com/Edward-Ji/DynamicArray/actions/workflows/test.yml)

An implementation of a dynamic array of void pointers in C language.

## Abstract

By providing an item free function when creating the array, you need not worry
about deallocating the memory manually when removing items from the array, the
functions will handle it for you. Being dynamic, the array automatically resizes
if it has insufficient or excess capacity. The implementation also mimics a
functional programming approach by accepting function pointers to iteratively
operate on data.

## Getting Started

### Installation

Simply download the source file and the header file to your project directory.
For example, with `wget`:

```
wget https://raw.githubusercontent.com/Edward-Ji/DynamicArray/main/darray.h
wget https://raw.githubusercontent.com/Edward-Ji/DynamicArray/main/darray.c
```

You can also download other files (e.g. `util/dtype.h`).

### Documentation

The full documentation is automatically deployed on [GitHub Pages].

### Conventions

All the dynamic array functions follow the rules below except for `new_darray`
and `del_darray`. These rules make it easier to memorize the functions and their
signature.

- They start with `darray_`.
- Their arguments usually appear in the following order:
    * A pointer to the dynamic array to operate on;
    * An index in that array;
    * An item to perform the operation with, or another array;
    * A function pointer for operation, aggregation, comparison etc.
- They usually return 1 on success and 0 on failure. Most functions set
  `darray_errno` on failure.

## Run From Source

This project uses GNU make utility to build and run the documentation,
demonstrations and test.

### Documentations

To generate the documentations locally, you need Doxygen. Read the
[Doxygen Manual] for an installation guide. Then, run `make doc` to generate
the documentations. To see the documentations, open `html/index.html` in the
browser.

### Demonstrations

Run `make demo` to compile the demonstration source files in the `demo`
directory. The executable will be in the `bin` directory. For example, there
will be a vector example, run it with `bin/vector`.

### Automated Testing

Run the following command to build and run the automated unit tests:

```
make test && bin/test
```

**After** passing all tests, use [Valgrind] to also check for memory leaks:

```
make test && \
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         bin/test
```

[Doxygen Manual]: https://www.doxygen.nl/manual/install.html
[GitHub Pages]: https://edward-ji.github.io/DynamicArray
[Valgrind]: https://valgrind.org
