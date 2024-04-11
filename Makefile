# C Strings
# The strings command written in C
# Github: https://www.github.com/0x4248/c_strings
# Licence: GNU General Public License v3.0
# By: 0x4248

CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I include

OUTPUT = strings

SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c,build/%.o,$(SRCS))

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

build/%.o: src/%.c | build
	@echo "CC $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build:
	@mkdir -p $@

clean:
	@rm -rf build $(OUTPUT)

.PHONY: all clean
