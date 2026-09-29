CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -Iinclude -g
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)
BIN = bin/shell

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ) | bin
	$(CC) $(CFLAGS) -o $@ $(OBJ)

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

obj:
	mkdir -p obj

clean:
	rm -rf obj bin
