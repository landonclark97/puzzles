#include <SDL2/SDL.h>

#include <stdlib.h>

#include "./stack.h"


#define ALPHA (2)


stack_t stack_new(int init_len) {
    stack_t s;
    s.data = calloc(init_len, sizeof(uint64_t));
    s.len = init_len;
    s.used = 0;
    s.mut = SDL_CreateMutex();
    return s;
}

void stack_insert(stack_t* s, uint64_t val) {
    SDL_LockMutex(s->mut);
    if (s->used == s->len) {
        s->data = realloc(s->data, sizeof(uint64_t)*s->len*ALPHA);
        s->len = s->len*ALPHA;
    }
    s->data[s->used] = val;
    s->used += 1;
    SDL_UnlockMutex(s->mut);
}

uint64_t stack_pop(stack_t* s) {
    SDL_LockMutex(s->mut);
    if (s->used == 0) {
        SDL_UnlockMutex(s->mut);
        return 0;
    }
    s->used -= 1;
    SDL_UnlockMutex(s->mut);
    return s->data[s->used];
}

void stack_free(stack_t* s) {
    SDL_DestroyMutex(s->mut);
    free(s->data);
}
