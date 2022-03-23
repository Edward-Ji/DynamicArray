CC = gcc
CFLAGS = -O1 -g -Wall -Werror
LDFLAGS = -fsanitize=address,leak -static-libasan

BINDIR := ./bin
OBJDIR := ./obj
DEMOSRC := $(shell find . -name 'demo_*.c')
TARGETS := $(DEMOSRC:%.c=$(BINDIR)/%)

debug: dirs $(TARGETS)

dirs:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

$(TARGETS): $(BINDIR)/%: $(OBJDIR)/%.o $(OBJDIR)/darray.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(BINDIR) $(OBJDIR)
