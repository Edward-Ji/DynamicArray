CC = gcc
CFLAGS = -O1 -g -Wall -Werror
LDFLAGS = -fsanitize=address,leak -static-libasan

BINDIR := ./bin/
OBJDIR := ./obj/
DEMOSRC := $(shell find . -name 'demo_*.c')
TARGETS := $(DEMOSRC:./%.c=$(BINDIR)%)

demo: $(BINDIR) $(OBJDIR) $(TARGETS)

./%/:
	mkdir -p $@

$(TARGETS): $(BINDIR)%: $(OBJDIR)%.o $(OBJDIR)darray.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY:
clean:
	rm -rf $(BINDIR) $(OBJDIR)
