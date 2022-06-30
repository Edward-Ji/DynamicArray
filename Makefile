CC := gcc
CFLAGS := -O2 -Wall -Werror

BINDIR := ./bin
OBJDIR := ./obj
DEMODIR := ./demo
HTMLDIR := ./html

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

doc: html

$(HTMLDIR): Doxyfile MAIN.md darray.c darray.h
	rm -rf $(HTMLDIR)
	doxygen

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(HTMLDIR)

.PHONY: doc, clean
