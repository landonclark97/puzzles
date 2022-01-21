#ifndef __INPUTHANDLER__
#define __INPUTHANDLER__

#include <SDL2/SDL.h>

#include "./input_state.h"

#include "../graphics/window.h"

#include "../common/threading.h"


puzzles_thread_t input_handler_init(window_t* win);

void input_handler_close(puzzles_thread_t thread);



#endif
