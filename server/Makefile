
CC = gcc
TARGET = aesdsocket
CROSS_COMPILE = aarch64-none-linux-gnu-

# check if CROSS_COMPILER is set or not
ifdef CROSS_COMPILE
    CC := $(CROSS_COMPILE)gcc
endif


# build application
all: $(TARGET)

$(TARGET): aesdsocket.o
	$(CC) aesdsocket.o -o $(TARGET)

aesdsocket.o: aesdsocket.c
	$(CC) -c aesdsocket.c -Wall -Werror

# removes the application and all .o files
clean:
	rm -f $(TARGET) *.o
