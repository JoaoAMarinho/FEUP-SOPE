CC=gcc

# Remainder to always resolve the compiler warnings
CFLAGS=-g -Wall -Wextra -pedantic #-Werror

BUILD_DIR = ./build
LIB_DIR = ./lib
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -name *.c)
LIB := $(shell find $(LIB_DIR) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
LIB_OBJS := $(LIB:%=$(BUILD_DIR)/%.o)

all: client server cptest

server: $(LIB_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -DDELAY=0 $^ -pthread -o s

CLIENT_DIR = ./mp22-code-v1
CLIENT := $(shell find $(CLIENT_DIR) -name *.c)
CLIENT_OBJS := $(CLIENT:%=$(BUILD_DIR)/%.o)

# In order to compile our client just go to
# the other folder and compile it from there

client: $(CLIENT_OBJS)
	$(CC) -Wall $^ -pthread -o c 

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

cptest:
	cp tests/test.sh ./

.PHONY: clean

clean:
	rm -rf c s $(BUILD_DIR)/* *.log *.txt
	rm -f test.sh

MKDIR_P = mkdir -p
