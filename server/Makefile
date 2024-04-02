CC := aarch64-none-linux-gnu-gcc
TARGET := aesdsocket
CFLAGS := -Wall -Wextra

all: $(TARGET)

$(TARGET): aesdsocket.o
	$(CC) aesdsocket.o -o $(TARGET)

aesdsocket.o: aesdsocket.c
	$(CC) -c aesdsocket.c $(CFLAGS)

clean:
	rm -f $(TARGET) *.o
