# DynamicArray

[![Automated Testing](https://github.com/Edward-Ji/DynamicArray/actions/workflows/test.yml/badge.svg)](https://github.com/Edward-Ji/DynamicArray/actions/workflows/test.yml)

An implementation of a dynamic array of void pointers in C language.

## Abstract

By providing an item free function when creating the array, you need not worry
about deallocating the memory manually when removing items from the array, the
functions will handle it for you. Being dynamic, the array automatically resizes
if it has insufficient or excess capacity. The implementation also mimics a
functional programming approach by accepting function pointers to iteratively
operate on data.

## Documentation

The documentation is automatically deployed on [GitHub Pages].

To generate the documentations locally, you need Doxygen. Read the
[Doxygen Manual] for an installation guide. Then, run `make doc` to generate
the documentations. The output will be in the `html` directory.

## Demonstration

Run `make demo` to compile the demonstration source files in the `demo`
directory. The executable will be in the `bin` directory. For example, there
will be a matrix example, run it with `bin/matrix`.

## Installation

Simply download the source file and header file to your project directory with

```
wget https://raw.githubusercontent.com/Edward-Ji/DynamicArray/main/darray.h
wget https://raw.githubusercontent.com/Edward-Ji/DynamicArray/main/darray.c
```

or

```
curl https://raw.githubusercontent.com/Edward-Ji/DynamicArray/main/darray.h \
    -o darray.h
curl https://raw.githubusercontent.com/Edward-Ji/DynamicArray/main/darray.c \
    -o darray.c
```

[Doxygen Manual]: https://www.doxygen.nl/manual/install.html
[GitHub Pages]: https://edward-ji.github.io/DynamicArray

## Testing

Run the following command to run the automated unit tests:

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

[valgrind]: https://valgrind.org
