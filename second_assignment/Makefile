CC=gcc
CFLAGS=-Wall

DEPS = 20182745.h
OBJ = 20182745.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
20182745: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)