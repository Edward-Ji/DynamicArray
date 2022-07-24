CC := gcc
CFLAGS := -O2 -Wall -Werror
CFLAGS_DEBUG := -g -Wall -Werror

BINDIR := ./bin
OBJDIR := ./obj
DEMODIR := ./demo
TESTDIR := ./test
HTMLDIR := ./html

DEMOSRC := $(shell find $(DEMODIR) -name '*.c')
DEMOEXE := $(DEMOSRC:$(DEMODIR)/%.c=$(BINDIR)/%)

TESTSRC := $(shell find $(TESTDIR) -name '*.c')
TESTEXE := $(TESTSRC:$(TESTDIR)/%.c=$(BINDIR)/%)

demo: $(DEMOEXE)

test: $(TESTEXE)

$(DEMOEXE): $(BINDIR)/%: $(OBJDIR)/darray.o $(OBJDIR)/demo_%.o
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(TESTEXE): $(BINDIR)/%: $(OBJDIR)/darray.o $(OBJDIR)/test_%.o
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/darray.o: darray.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/demo_%.o: $(DEMODIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/test_%.o: $(TESTDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

doc: html

$(HTMLDIR): Doxyfile MAIN.md darray.c darray.h
	rm -rf $(HTMLDIR)
	doxygen

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(HTMLDIR)

.PHONY: doc, clean
