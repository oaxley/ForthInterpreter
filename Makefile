# @file		Makefile
# @author	Sebastien LEGRAND
#
# @brief	Makefile to build the Forth Interpreter

# ----- constants
CC := g++
CFLAGS := -Wall -Werror -Wextra -Weffc++ -std=c++20
LFLAGS :=

BIN_DIR := bin
SRC_DIR := src

TARGET := $(BIN_DIR)/slForth

# define the sources and objects variables
SOURCES := $(wildcard $(SRC_DIR)/*.cc)
OBJECTS := $(patsubst $(SRC_DIR)/%.cc, $(BIN_DIR)/%.o, $(SOURCES))

.PHONY: all clean run

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	@ mkdir -f $@

$(TARGET): $(OBJECTS)
	@ $(CC) $(LFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cc
	@ $(CC) $(CFLAGS) -c $^ -o $@

clean:
	@ rm -f $(BIN_DIR)/*.o
	@ rm -f $(TARGET)

run:
	@$(TARGET)
