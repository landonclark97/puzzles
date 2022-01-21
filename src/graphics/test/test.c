#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "../window.h"
#include "../texture.h"
#include "../sprite.h"

//Screen dimension constants
const int SCREEN_WIDTH = 920;
const int SCREEN_HEIGHT = 740;

int main( int argc, char* args[] )
{
    //The window we'll be rendering to
    window_t window;

    if (!window_init(&window, "Graphics testing", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        window_free(&window);
        return -1;
    }

    sprite_t* spr = sprite_new(&window, "skeleton_sheet.png", 4, 9, 10);

    SDL_Event e;

    int x,y;
    x = 0;
    y = 0;

    bool up, down, left, right, space;
    up = false;
    down = false;
    left = false;
    right = false;
    space = false;

    float angle = 0.0;
    float scale = 1.0;

    bool exit = false;
    while(!exit) {

        while (SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                exit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_w:
                        up = true;
                        break;
                    case SDLK_s:
                        down = true;
                        break;
                    case SDLK_a:
                        left = true;
                        break;
                    case SDLK_d:
                        right = true;
                        break;
                    case SDLK_SPACE:
                        space = true;
                        break;
                }
            }
            else if (e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym) {
                    case SDLK_w:
                        up = false;
                        break;
                    case SDLK_s:
                        down = false;
                        break;
                    case SDLK_a:
                        left = false;
                        break;
                    case SDLK_d:
                        right = false;
                        break;
                    case SDLK_SPACE:
                        space = false;
                        break;
                }
            }
        }
        if (up) {
            y -= 1;
            sprite_set_row(spr, 0);
        }
        if (down) {
            y += 1;
            sprite_set_row(spr, 2);
        }
        if (left) {
            x -= 1;
            sprite_set_row(spr, 1);
        }
        if (right) {
            x += 1;
            sprite_set_row(spr, 3);
        }
        if (space) {
            /* angle += 5.0; */
            /* scale += 0.05; */
            sprite_update(spr);
        } else {
            if (scale > 1.0) scale -= 0.1;
        }

        sprite_render(spr, &window, x, y);
        window_render(&window);
    }

    sprite_free(&spr);
    window_free(&window);

    return 0;
}
