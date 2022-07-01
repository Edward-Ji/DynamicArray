# DynamicArray

Dynamic array of void pointers with automatic memory free in C language.

## Documentation

The documentation is automatically deployed on [GitHub Pages].

To generate the documentations locally, you need Doxygen.  Read the [Doxygen
Manual] for an installation guide. Then, run `make doc` to generate
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
