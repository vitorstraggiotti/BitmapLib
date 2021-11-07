
CC = gcc
CFLAGS = -Wall -pedantic -O2
CLIBS = -lm

PROGNAME = test

.PHONY: all clean

all:
	@echo "Make options:"
	@echo "  make test --> build test program"
	@echo "  make clean --> erase program and build files"


$(PROGNAME): bitmap.o test.o
	$(CC) -o $@ $^ $(CLIBS)

test.o: test.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
bitmap.o: bitmap.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
	
clean:
	rm $(PROGNAME) *.o saida*
