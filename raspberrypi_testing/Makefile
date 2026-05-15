CC = cc
CFLAGS = -Wall -Werror -Wpedantic
INCLUDES := -I./include -I/usr/include/libnl3
LD_FLAGS := -lpcap -lpthread -lnl-3 -lnl-genl-3

BUILD_DIR = build

SRCS    := $(wildcard src/*.c)
OBJS    := $(SRCS:.c=.o)
TARGET = main

.PHONY: all slink clean

all: $(TARGET) slink

$(TARGET): $(OBJS)
	@mkdir -p build
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(CFLAGS) $(OBJS) $(LD_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(LD_FLAGS)

slink: $(TARGET)
	ln -sf $(BUILD_DIR)/$(TARGET) .

clean:
	rm -rf $(OBJS)
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET)
