#ifndef __TEXTURE__
#define __TEXTURE__


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./window.h"


typedef struct {
    window_t* win;
    SDL_Texture* texture;
    int width, height;
} texture_t;


texture_t texture_from_png(window_t* win, const char* path);

void texture_render(texture_t* tex, float _x, float _y);

void texture_render_ex(texture_t* tex, float _x, float _y, float angle);

void texture_free(texture_t* tex);



#endif
