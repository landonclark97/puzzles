CFLAGS = -O3
LIB_PATH =-L/home/landon/.local/lib/
LIBS =-lSDL2 -lSDL2_image -lSDL2_mixer -lchipmunk -lflecs -ludev -lm -lpuzzles

INCLUDE = ../game/*.h ../../include/puzzles.h
C = ../game/*.c

DEBUG =-g

CC =gcc

test: test.c
	$(CC) $(CFLAGS) $(INCLUDE) $(C) $? -o $@ $(LIB_PATH) $(LIBS)
	./test
	rm ./test

build: test.c
	$(CC) $(CFLAGS) $(INCLUDE) $(C) $? -o $@ $(LIB_PATH) $(LIBS)


debug: test.c
	$(CC) $(DEBUG) $(CFLAGS) $(INCLUDE) $(C) $? -o $@ $(LIBS)

clean:
	rm ./test
	rm ./debug
