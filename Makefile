CC := gcc
CFLAGS := -O2 -Wall -Werror
CFLAGS_DEBUG := -g -Wall -Werror
LDFLAGS := -lm

BIN_DIR := ./bin
OBJ_DIR := ./obj
DEMO_DIR := ./demo
TEST_DIR := ./test
HTML_DIR := ./html

DEMO_SRC := $(shell find $(DEMO_DIR) -name '*.c')
DEMO_EXE := $(DEMO_SRC:$(DEMO_DIR)/%.c=$(BIN_DIR)/%)

TEST_SRC := $(shell find $(TEST_DIR) -name '*.c')
TEST_EXE := $(TEST_SRC:$(TEST_DIR)/%.c=$(BIN_DIR)/%)

all: demo test

demo: $(DEMO_EXE)

test: $(TEST_EXE)

$(DEMO_EXE): $(BIN_DIR)/%: $(OBJ_DIR)/darray.o $(OBJ_DIR)/demo_%.o
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TEST_EXE): $(BIN_DIR)/%: $(OBJ_DIR)/darray.o $(OBJ_DIR)/test_%.o
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/darray.o: darray.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ_DIR)/demo_%.o: $(DEMO_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS_DEBUG) -c $^ -o $@

doc: $(HTML_DIR)

$(HTML_DIR):
	doxygen

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(HTML_DIR)

.PHONY: html clean
