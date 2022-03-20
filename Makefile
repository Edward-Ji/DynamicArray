all: main.c darray.c darray.h
	gcc -o main -Wall -Werror main.c darray.c
test: main.c darray.c darray.h
	gcc -O1 -g -fsanitize=address -o main -Wall -Werror main.c darray.c
