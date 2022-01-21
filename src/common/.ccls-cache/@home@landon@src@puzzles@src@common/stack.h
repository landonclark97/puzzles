#ifndef __QUEUE__
#define __QUEUE__


#include <SDL2/SDL.h>

#include <stdint.h>


#define INIT_STACK_LEN (100)


typedef struct {
    uint64_t* data;
    int len, used;
    SDL_mutex* mut;
} stack_t;


#define STACK_NEW() stack_new(INIT_STACK_LEN)

stack_t stack_new(int init_len);

void stack_insert(stack_t* s, uint64_t val);

uint64_t stack_pop(stack_t* s);

void stack_free(stack_t* s);




#endif
