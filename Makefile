# rps - rock paper scissors

CC       := gcc
CSTD     := -std=gnu17
CFLAGS   := $(CSTD)
CFLAGS 	 += -Wall
CFLAGS   += -Wextra
CFLAGS   += -Wpedantic
CFLAGS   += -O2
CFLAGS   += -g
CFLAGS   += -fno-omit-frame-pointer
CFLAGS 	 += -D_POSIX_C_SOURCE=200809L
CFLAGS   += -I./include

LDFLAGS  :=

# Sanitizer flags (opt-in via `make build SANITIZE=1`)
ifdef SANITIZE
	CFLAGS  += -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
endif

# Source and object files
ROOT_DIR  := .
SRC_DIR   := ${ROOT_DIR}/src
INC_DIR   := ${ROOT_DIR}/include
BIN_DIR	  := ${ROOT_DIR}/bin
BUILD_DIR := ${ROOT_DIR}/build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := ${BIN_DIR}/rps

# Default target
.PHONY: all
all: build

# Build the binary
.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

# Debug build (with sanitizers)
.PHONY: debug
debug:
	$(MAKE) build SANITIZE=1

# Run the binary
.PHONY: run
run: build
	./$(TARGET) $(FILE)

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET)

# Show help
.PHONY: help
help:
	@echo "rps Makefile targets:"
	@echo "  build        - Build the binary (default)"
	@echo "  debug        - Build with sanitizers"
	@echo "  run FILE=x   - Build and run with file x"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help"

# Line count
.PHONY: linecount
linecount:
	@find . -type f \( -name "*.c" -o -name "*.h" \) -exec wc -l {} +
