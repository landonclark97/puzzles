#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <stdlib.h>
#include <stdbool.h>

#include "./window.h"
#include "./camera.h"


bool window_init(window_t* win, const char* title, int width, int height, uint32_t flags) {
    win->window = NULL;
    win->renderer = NULL;
    win->_fullscreen = false;
    win->_exit = false;
    win->_vsync = false;

    win->width = width;
    win->height = height;

    camera_init(&win->cam, (float) width/2.0, (float) height/2.0);

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    char* quality_flag = "0";
    if (flags & FILTER_LINEAR) {
        quality_flag = "1";
    }
    else if (flags & FILTER_ANISOTROPIC) {
        quality_flag = "2";
    }
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, quality_flag) < 0) {
        printf( "SDL could not enable linear filtering" );
    }

    win->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win->window == NULL) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    uint32_t vsync_flag = 0x0;
    if (flags & VSYNC_ON) {
        vsync_flag = SDL_RENDERER_PRESENTVSYNC;
        win->_vsync = true;
    }
    win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED | vsync_flag);
    if (win->renderer == NULL) {
        printf("Renderer failed to be created. SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    win->max_width = dm.w;
    win->max_height = dm.h;
    win->refresh = (float) dm.refresh_rate;
    win->scale = ((float) win->max_height)/540.0;

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("Error initializing SDL image. SDL_Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void window_toggle_fullscreen(window_t* win) {
    if (!win->_fullscreen) {
        SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        win->_fullscreen = true;
    } else {
        SDL_SetWindowFullscreen(win->window, 0);
        win->_fullscreen = false;
    }
}

void window_resize(window_t* win) {
    camera_resize(&win->cam, (float) win->width/2.0, (float) win->height/2.0);
}

void window_render_clear(window_t* win) {
    SDL_SetRenderDrawColor(win->renderer, 0x00, 0x0F, 0x0F, 0x0F);
    SDL_RenderClear(win->renderer);
}

void window_render_present(window_t* win) {
    SDL_RenderPresent(win->renderer);
}

void window_free(window_t* win) {
    if (win->renderer)
        SDL_DestroyRenderer(win->renderer);
    if (win->window)
        SDL_DestroyWindow(win->window);

    IMG_Quit();
    SDL_Quit();
}

