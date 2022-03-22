CC = gcc
CFLAGS = -O1 -g -Wall -Werror
LDFLAGS = -fsanitize=address,leak

BINDIR := ./bin
SRC := $(shell find . -name 'demo_*.c')
TARGETS := $(SRC:%.c=$(BINDIR)/%)

debug: $(BINDIR) $(TARGETS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGETS): $(BINDIR)/%: %.o darray.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BINDIR) *.o
