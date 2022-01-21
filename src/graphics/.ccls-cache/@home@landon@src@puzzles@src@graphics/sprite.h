#ifndef __SPRITE__
#define __SPRITE__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./texture.h"
#include "./window.h"


typedef struct {
    texture_t* tex;
    window_t* win;
    int horiz_frames;
    int vert_frames;
    int sprite_index[2];
    float anim_time;
    float anim_acc;
} sprite_t;


sprite_t sprite_new(texture_t* tex, int h_frames, int v_frames, float anim_time);

void sprite_refresh(sprite_t* spr);

void sprite_set_row(sprite_t* spr, int row);

void sprite_set_col(sprite_t* spr, int col);

void sprite_update(sprite_t* spr, double dt);

void sprite_render(sprite_t* spr, float _x, float _y, double dt);

void sprite_render_ex(sprite_t* spr, float _x, float _y, float _scale, float angle, double dt);

void sprite_free(sprite_t* spr);




#endif
