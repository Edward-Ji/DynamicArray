CC = gcc
CFLAGS = -O1 -g -Wall -Werror
LDFLAGS = -fsanitize=address,leak

TARGETS = demo_matrix demo_stu demo_vector

debug: bin $(TARGETS)

bin:
	mkdir bin

$(TARGETS): %: %.c darray.c
	$(CC) $(LDFLAGS) $^ -o bin/$@

clean:
	rm -rf bin *.o
