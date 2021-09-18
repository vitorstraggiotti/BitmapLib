
CC = gcc
CFLAGS = -Wall -pedantic -O2
CLIBS = -lm

PROGNAME = test

.PHONY: all clean

all:
	@echo "Make options:"
	@echo "  make test --> build test program"
	@echo "  make clean --> erase program and build files"


$(PROGNAME): ./build/bitmap.o \
             ./build/comp_vision.o \
             ./build/load_animation_v1.0.o \
             ./build/test.o
	$(CC) -o $@ $^ $(CLIBS)

./build/test.o: ./examples/test.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
./build/bitmap.o: ./source/bitmap.c
	$(CC) $(CFLAGS) -c -o $@ $^

./build/comp_vision.o: ./source/comp_vision.c
	$(CC) $(CFLAGS) -c -o $@ $^

./build/load_animation_v1.0.o: ./source/load_animation_v1.0.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
	
clean:
	rm -rf ./build/* $(PROGNAME)
