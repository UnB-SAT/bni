# Compiler and flags
CC = gcc
OPT = -O3
CFLAGS = -Iinclude
# PDDL_FLAGS = -I$(CURDIR)
DEBUG_FLAGS = -ggdb3
RUN_FLAG := $(OPT)
READLINE_FLAGS = -lreadline

# Source and object files
SRC_DIR := src
REPL_DIR := src
VAL_DIR := src
SRC := $(SRC_DIR)/parser.c $(SRC_DIR)/linked_list.c $(SRC_DIR)/stack.c $(SRC_DIR)/symbol_table.c
REPL_SRC := $(REPL_DIR)/repl.c
VAL_SRC := $(VAL_DIR)/val.c
OBJ = $(SRC:.c=.o)
TARGET = parser
PREFIX := $(CURDIR)

# Default values for PDDL files (override via command line)
OUTPUT_DIR := $(PREFIX)/bni.out
PDDL_FILE := $(OUTPUT_DIR)/pddl.c
PDDL_HEADER := $(OUTPUT_DIR)/pddl.h

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ)
	$(CC) $(RUN_FLAG) $(OBJ) -o $@

# Compile source files to object files
%.o: %.c
	$(CC) $(RUN_FLAG) $(CFLAGS) -c $< -o $@

# Complile REPL and pddl.c
.PHONY: repl
repl:
	$(CC) $(RUN_FLAG) -I$(OUTPUT_DIR) $(PDDL_FILE) -DPDDL_HEADER=\"$(PDDL_HEADER)\" $(REPL_SRC) $(READLINE_FLAGS) -o $(PREFIX)/$@
	# $(CC) $(RUN_FLAG) $(CURDIR)/pddl.c $(PDDL_FLAGS) $(REPL_SRC) $(READLINE_FLAGS) -o $(PREFIX)/$@

# Complile VAL and pddl.c
.PHONY: val
val:
	$(CC) $(RUN_FLAG) -I$(OUTPUT_DIR) $(PDDL_FILE) -DPDDL_HEADER=\"$(PDDL_HEADER)\" $(VAL_SRC) -o $(PREFIX)/$@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET) repl pddl.c pddl.h

# Run with valgrind
valgrind:
	valgrind --leak-check=full ./parser test/domain-fga.pddl test/problem-fga.pddl /tmp/pddl.c /tmp/pddl.h

# Run without valgrind
rsk:
	./parser test/domain-snake.pddl test/problem-snake.pddl /tmp/pddl.c /tmp/pddl.h
rfg:
	./parser test/domain-fga.pddl test/problem-fga.pddl /tmp/pddl.c /tmp/pddl.h
