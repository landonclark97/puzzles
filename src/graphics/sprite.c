#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <stdio.h>

#include "./sprite.h"
#include "./window.h"
#include "./camera.h"


sprite_t sprite_new(texture_t* tex, int h_frames, int v_frames, float anim_time) {

    sprite_t spr;

    spr.tex = tex;
    spr.win = tex->win;
    spr.horiz_frames = h_frames;
    spr.vert_frames = v_frames;
    spr.sprite_index[0] = 0;
    spr.sprite_index[1] = 0;
    spr.anim_time = anim_time;
    spr.anim_acc = 0.0;

    return spr;
}

void sprite_refresh(sprite_t* spr) {
    sprite_set_col(spr, 0);
    spr->anim_acc = 0.0;
}

void sprite_set_row(sprite_t* spr, int row) {
    spr->sprite_index[1] = row;
}

void sprite_set_col(sprite_t* spr, int col) {
    spr->sprite_index[0] = col;
}

void sprite_update(sprite_t* spr, double dt) {
    spr->anim_acc += dt;
    if (spr->anim_acc >= spr->anim_time) {
        spr->anim_acc -= spr->anim_time;

        int i = spr->sprite_index[0];
        if (i + 1 >= spr->vert_frames) {
            spr->sprite_index[0] = 0;
        }
        else {
            spr->sprite_index[0] += 1;
        }
    }
}

void sprite_render(sprite_t* spr, float _x, float _y, double dt) {

    sprite_update(spr, dt);

    int x_lim = spr->tex->width;
    int y_lim = spr->tex->height;

    int width = x_lim/spr->vert_frames;
    int height = y_lim/spr->horiz_frames;

    float fx = _x - (float) (width/2.0);
    float fy = _y - (float) (height/2.0);

    camera_frame(&spr->win->cam, &fx, &fy);

    int x = (int) fx;
    int y = (int) fy;

    int tex_x = spr->sprite_index[0]*width;
    int tex_y = spr->sprite_index[1]*height;

    SDL_Rect rect = {x, y, x+width, y+height};
    SDL_Rect curr_sprite = {tex_x, tex_y, width, height};

    SDL_RenderCopy(spr->win->renderer, spr->tex->texture, &curr_sprite, &rect);
}

void sprite_render_ex(sprite_t* spr, float _x, float _y, float _scale, float angle, double dt) {

    sprite_update(spr, dt);

    float scale = spr->win->scale*_scale;

    int x_lim = spr->tex->width;
    int y_lim = spr->tex->height;

    int width = x_lim/spr->vert_frames;
    int height = y_lim/spr->horiz_frames;

    int h_width = width/2;
    int h_height = height/2;

    float fx = _x - (float) h_width;
    float fy = _y - (float) h_height;

    camera_frame(&spr->win->cam, &fx, &fy);

    int x = (int) fx;
    int y = (int) fy;

    int tex_x = spr->sprite_index[0]*width;
    int tex_y = spr->sprite_index[1]*height;

    int new_x = (x + h_width) - (h_width * scale);
    int new_y = (y + h_height) - (h_height * scale);

    SDL_Point center = {h_width * scale, h_height * scale};
    SDL_Rect rect = {new_x, new_y, width * scale, height * scale};
    SDL_Rect curr_sprite = {tex_x, tex_y, width, height};

    SDL_RenderCopyEx(spr->win->renderer, spr->tex->texture, &curr_sprite, &rect, (double) angle, &center, SDL_FLIP_NONE);
}

void sprite_free(sprite_t* spr) {
    void* none;
}
