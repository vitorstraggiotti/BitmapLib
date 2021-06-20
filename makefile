# build test binary for image processing libraries

all: test

test: bitmap.o comp_vision.o load_animation_v1.0.o test.o
	gcc bitmap.o comp_vision.o load_animation_v1.0.o test.o -lm -o test

test.o: test.c
	gcc -c test.c -o test.o
	
bitmap.o: bitmap.c bitmap.h
	gcc -c bitmap.c -o bitmap.o

comp_vision.o: comp_vision.c comp_vision.h
	gcc -c comp_vision.c -o comp_vision.o

load_animation_v1.0.o: load_animation_v1.0.c load_animation_v1.0.h
	gcc -c load_animation_v1.0.c -o load_animation_v1.0.o
	
	
clean:
	rm -f *.o test
