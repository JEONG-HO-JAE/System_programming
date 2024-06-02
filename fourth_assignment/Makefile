CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm

all: 20182745-cv 20182745-mut 20182745-sem

20182745-cv: 20182745-cv.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

20182745-mut: 20182745-mut.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

20182745-sem: 20182745-sem.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f 20182745-cv 20182745-mut 20182745-sem