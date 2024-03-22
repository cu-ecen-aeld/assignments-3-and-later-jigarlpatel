# Compiler options
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra

# Target executable
TARGET = writer

# Source files
SRCS = writer.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
.PHONY: all
	all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
