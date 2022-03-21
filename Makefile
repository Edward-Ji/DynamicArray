CC = gcc
CFLAGS = -O1 -g -Wall -Werror
LDFLAGS = -fsanitize=address,leak

debug: bin demo_matrix demo_stu

bin:
	mkdir bin

demo_matrix: demo_matrix.o darray.o
	$(CC) $(LDFLAGS) $^ -o bin/$@

demo_stu: demo_stu.o darray.o
	$(CC) $(LDFLAGS) $^ -o bin/$@

clean:
	rm -rf bin *.o
