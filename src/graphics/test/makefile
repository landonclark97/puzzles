CFLAGS = -O3
LIBS =-lSDL2 -lSDL2_image

INCLUDE = ../window.h ../texture.h ../sprite.h
C = ../window.c ../texture.c ../sprite.c

CC =gcc

test: test.c
	$(CC) $(CFLAGS) $(INCLUDE) $(C) $? -o $@ $(LIBS)
	./test
	rm ./test

clean:
	rm ./test
