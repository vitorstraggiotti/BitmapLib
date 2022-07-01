
CC = gcc
CFLAGS = -Wall -pedantic -O2 -c
D_CFLAGS = -Wall -pedantic -g -c

PROGNAME = test

.PHONY: all clean

all:
	@echo "Make options:"
	@echo "  make test    --> build test program"
	@echo "  make testGDB --> build test debug version"
	@echo "  make clean   --> erase program and build files"


# Optimized version
test: bitmap.o test.o
	$(CC) -o $@ $^

test.o: test.c
	$(CC) $(CFLAGS) -o $@ $^
	
bitmap.o: bitmap.c
	$(CC) $(CFLAGS) -o $@ $^

# Debug version
testGDB: bitmap_d.o test_d.o
	$(CC) -o $@ $^ $(CLIBS)

test_d.o: test.c
	$(CC) $(D_CFLAGS) -o $@ $^
	
bitmap_d.o: bitmap.c
	$(CC) $(D_CFLAGS) -o $@ $^

clean:
	rm test testGDB *.o out*
