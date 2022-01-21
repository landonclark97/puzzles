CFLAGS = -O3 -fPIC
SHARED = -O3 -shared -o
LIBS =-lSDL2 -lSDL2_image -lSDL2_mixer -lchipmunk -lflecs -ludev -lm

INCLUDE = ./*/*.h

DEBUG =-g

CC =gcc

.PHONY : install
install: \
	build/o/physics.o \
	build/o/input_handler.o \
	build/o/input_state.o \
	build/o/camera.o \
	build/o/sprite.o \
	build/o/texture.o \
	build/o/window.o \
	build/o/data_handler.o \
	build/o/dict.o \
	build/o/stack.o \
	build/o/ticker.o \
	build/o/audio.o \
	lib
	mv build/libpuzzles.so $HOME/.local/lib/

build/o/physics.o: src/physics/physics.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/input_handler.o: src/input/input_handler.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/input_state.o: src/input/input_state.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/camera.o: src/graphics/camera.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/sprite.o: src/graphics/sprite.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/texture.o: src/graphics/texture.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/window.o: src/graphics/window.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/data_handler.o: src/data/data_handler.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/dict.o: src/common/dict.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/stack.o: src/common/stack.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/ticker.o: src/common/ticker.c
	$(CC) $(CFLAGS) -o $@  -c $?

build/o/audio.o: src/audio/audio.c
	$(CC) $(CFLAGS) -o $@  -c $?

.PHONY : lib
lib:
	$(CC) $(SHARED) build/libpuzzles.so build/o/*
