CC=gcc
CFLAGS=-Wall
TARGET=bitreader

all: $(TARGET)

$(TARGET): bitreader.c
	$(CC) $(CFLAGS) -o $(TARGET) bitreader.c

clean:
	rm -f $(TARGET)