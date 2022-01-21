#ifndef __WINDOW__
#define __WINDOW__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

#include "./camera.h"


#define VSYNC_ON (0x1)
#define VSYNC_OFF (0x0)

#define FILTER_OFF (0x00)
#define FILTER_LINEAR (0x10)
#define FILTER_ANISOTROPIC (0x20)


typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;

    int width, height;
    int max_width, max_height;
    camera_t cam;
    float scale;
    float refresh;
    bool _fullscreen;
    bool _exit;
    bool _vsync;
} window_t;


bool window_init(window_t* win, const char* title, int width, int height, uint32_t flags);

void window_toggle_fullscreen(window_t* win);

void window_toggle_vsync(window_t* win);

void window_resize(window_t* win);

void window_render_clear(window_t* win);

void window_render_present(window_t* win);

void window_free(window_t* win);



#endif
