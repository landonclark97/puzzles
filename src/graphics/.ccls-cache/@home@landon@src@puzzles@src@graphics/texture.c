#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>

#include "./texture.h"
#include "./window.h"


texture_t texture_from_png(window_t* win, const char* path) {
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        return (texture_t) {};
    }

    int w, h;
    w = surf->w;
    h = surf->h;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(win->renderer, surf);
    SDL_FreeSurface(surf);
    if (!texture) {
        return (texture_t) {};
    }

    texture_t tex;
    tex.win = win;
    tex.texture = texture;
    tex.width = w;
    tex.height = h;

    return tex;
}

void texture_render(texture_t* tex, float _x, float _y) {

    float fx = _x - (float) (tex->width/2.0);
    float fy = _y - (float) (tex->height/2.0);

    camera_frame(&tex->win->cam, &fx, &fy);

    int x = (int) fx;
    int y = (int) fy;

    SDL_Rect rect = {x, y, tex->width, tex->height};

    SDL_RenderCopy(tex->win->renderer, tex->texture, NULL, &rect);
}

void texture_render_ex(texture_t* tex, float _x, float _y, float angle) {

    float scale = tex->win->scale;

    float fx = _x - (float) (tex->width/2.0);
    float fy = _y - (float) (tex->height/2.0);

    camera_frame(&tex->win->cam, &fx, &fy);

    int x = (int) fx;
    int y = (int) fy;

    int new_x = (x + tex->width/2) - (tex->width/2 * scale);
    int new_y = (y + tex->height/2) - (tex->height/2 * scale);

    SDL_Point center = {tex->width/2 * scale, tex->height/2 * scale};
    SDL_Rect rect = { new_x, new_y, tex->width * scale, tex->height * scale };
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_RenderCopyEx(tex->win->renderer, tex->texture, NULL, &rect, (double) angle, &center, flip);
}

void texture_free(texture_t* tex) {
    SDL_DestroyTexture(tex->texture);
}
