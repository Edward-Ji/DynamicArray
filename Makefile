CC := gcc
CFLAGS := -O2 -Wall -Werror
LDFLAGS := 

BINDIR := ./bin
OBJDIR := ./obj
DEMODIR := ./demo

DEMOSRC := $(shell find $(DEMODIR) -name '*.c')
TARGETS := $(DEMOSRC:$(DEMODIR)/%.c=$(BINDIR)/%)

demo: $(TARGETS)

$(TARGETS): $(BINDIR)/%: $(OBJDIR)/darray.o $(OBJDIR)/%.o
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/darray.o: darray.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%.o: $(DEMODIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY:
clean:
	rm -rf $(BINDIR) $(OBJDIR)
