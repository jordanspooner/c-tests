CC     = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -O3

.PHONY: all clean

all: heapsort

heapsort: heapsort.o binaryheap.o

heapsort.o: binaryheap.h
binaryheap.o: binaryheap.h

clean:
	rm -f *.o heapsort