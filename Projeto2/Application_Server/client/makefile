CC=gcc

# Remainder to always resolve the compiler warnings
CFLAGS=-g -Wall -Wextra -Werror -pedantic

BUILD_DIR = ./build
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

all: client server cptest

client: $(OBJS)
	$(CC) $(CFLAGS) $^ -pthread -o c

SERVER_DIR = ./mp21-code-v4

server: $(SERVER_DIR)/*.o $(SERVER_DIR)/*.c $(SERVER_DIR)/*.h
	gcc -Wall -DDELAY=0 -o s $(SERVER_DIR)/*.o $(SERVER_DIR)/*.c -pthread

cptest:
	cp tests/test_client.sh ./

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf c s $(BUILD_DIR)/* *.log
	rm -f test_client.sh
	rm -f *.txt


MKDIR_P = mkdir -p
