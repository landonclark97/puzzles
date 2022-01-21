#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "./input_handler.h"
#include "./input_state.h"

#include "../graphics/window.h"

#include "../common/threading.h"


int input_event_loop(void* d);

void add_controller(input_state_t* in_state, SDL_Event* e);

void remove_controller(input_state_t* in_state);


puzzles_thread_t input_handler_init(window_t* win) {
    return puzzles_thread_new((puzzles_thread_fn_t)input_event_loop, win);
}

int input_event_loop(void* d) {

    input_state_init();

    SDL_Event e;
    window_t* win = d;
    input_state_t* in_state = input_state_get();

    while (!(win->_exit)) {
        while (SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                win->_exit = true;
                break;
            }
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(win->window, &win->width, &win->height);
                    window_resize(win);
                }
            }
            else if (e.type == SDL_CONTROLLERDEVICEADDED) {
                fprintf(stderr, "adding controller\n");
                add_controller(in_state, &e);
                input_state_zero();
            }
            else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
                fprintf(stderr, "removing controller\n");
                remove_controller(in_state);
                input_state_zero();
            }
            else if (in_state->input_mode == KEYBOARD_MODE) {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_w:
                            in_state->inputs[W] = true;
                            break;
                        case SDLK_s:
                            in_state->inputs[S] = true;
                            break;
                        case SDLK_a:
                            in_state->inputs[A] = true;
                            break;
                        case SDLK_d:
                            in_state->inputs[D] = true;
                            break;
                        case SDLK_q:
                            in_state->inputs[Q] = true;
                            break;
                        case SDLK_e:
                            in_state->inputs[E] = true;
                            break;
                        case SDLK_r:
                            in_state->inputs[R] = true;
                            break;
                        case SDLK_f:
                            in_state->inputs[F] = true;
                            break;
                        case SDLK_t:
                            in_state->inputs[T] = true;
                            break;
                        case SDLK_z:
                            in_state->inputs[Z] = true;
                            break;
                        case SDLK_x:
                            in_state->inputs[X] = true;
                            break;
                        case SDLK_c:
                            in_state->inputs[C] = true;
                            break;
                        case SDLK_SPACE:
                            in_state->inputs[SPACE] = true;
                            break;
                        case SDLK_LSHIFT:
                            in_state->inputs[LSHIFT] = true;
                            break;
                        case SDLK_LCTRL:
                            in_state->inputs[LCTRL] = true;
                            break;
                        case SDLK_LALT:
                            in_state->inputs[LALT] = true;
                            break;
                        case SDLK_RETURN:
                            in_state->inputs[RETURN] = true;
                            window_toggle_fullscreen(win);
                            break;
                    }
                }
                else if (e.type == SDL_KEYUP) {
                    switch (e.key.keysym.sym) {
                        case SDLK_w:
                            in_state->inputs[W] = false;
                            break;
                        case SDLK_s:
                            in_state->inputs[S] = false;
                            break;
                        case SDLK_a:
                            in_state->inputs[A] = false;
                            break;
                        case SDLK_d:
                            in_state->inputs[D] = false;
                            break;
                        case SDLK_q:
                            in_state->inputs[Q] = false;
                            break;
                        case SDLK_e:
                            in_state->inputs[E] = false;
                            break;
                        case SDLK_r:
                            in_state->inputs[R] = false;
                            break;
                        case SDLK_f:
                            in_state->inputs[F] = false;
                            break;
                        case SDLK_t:
                            in_state->inputs[T] = false;
                            break;
                        case SDLK_z:
                            in_state->inputs[Z] = false;
                            break;
                        case SDLK_x:
                            in_state->inputs[X] = false;
                            break;
                        case SDLK_c:
                            in_state->inputs[C] = false;
                            break;
                        case SDLK_SPACE:
                            in_state->inputs[SPACE] = false;
                            break;
                        case SDLK_LSHIFT:
                            in_state->inputs[LSHIFT] = false;
                            break;
                        case SDLK_LCTRL:
                            in_state->inputs[LCTRL] = false;
                            break;
                        case SDLK_LALT:
                            in_state->inputs[LALT] = false;
                            break;
                        case SDLK_RETURN:
                            in_state->inputs[RETURN] = false;
                            break;
                    }
                }
            }
            else if (in_state->input_mode == JOYSTICK_MODE) {
                if (e.type == SDL_CONTROLLERBUTTONDOWN) {
                    in_state->inputs[e.cbutton.button] = true;
                }
                else if (e.type == SDL_CONTROLLERBUTTONUP) {
                    in_state->inputs[e.cbutton.button] = false;
                }
                else if (e.type == SDL_CONTROLLERAXISMOTION) {
                    if (e.caxis.value >= in_state->gc_deadzone || e.caxis.value <= -in_state->gc_deadzone) {
                        in_state->analog_inputs[e.caxis.axis] = e.caxis.value;
                    } else {
                        in_state->analog_inputs[e.caxis.axis] = 0;
                    }
                }
            }
        }
    }

    fprintf(stderr, "\nleaving with event: %d\n", e.type);

    return 0;
}

void input_handler_close(puzzles_thread_t thread) {
    puzzles_thread_join(thread);
}

void add_controller(input_state_t* in_state, SDL_Event* e) {
    if (!(in_state->input_mode == JOYSTICK_MODE)) {
        int dev_index = e->cdevice.which;
        in_state->input_mode = JOYSTICK_MODE;
        in_state->gc = SDL_GameControllerOpen(dev_index);
        if (!in_state->gc) {
            fprintf(stderr, "SDL could not connect controller");
            in_state->input_mode = KEYBOARD_MODE;
        }
    }
}

void remove_controller(input_state_t* in_state) {
    if (in_state->gc) {
        SDL_GameControllerClose(in_state->gc);
        in_state->gc = NULL;
        in_state->input_mode = KEYBOARD_MODE;
    }
}
