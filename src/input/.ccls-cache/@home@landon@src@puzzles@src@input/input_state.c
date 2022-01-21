#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "./input_state.h"



static input_state_t in_state;


void input_state_init() {
    in_state.gc = NULL;
    in_state.gc_deadzone = GC_DEADZONE;
    for (int i = 0; i < INPUT_CNT; i++) {
        in_state.inputs[i] = false;
    }
    for (int i = 0; i < ANALOG_CNT; i++) {
        in_state.analog_inputs[i] = 0;
    }

    if (SDL_NumJoysticks() > 0) {
        in_state.input_mode = JOYSTICK_MODE;
        in_state.gc = SDL_GameControllerOpen(0);
        if (!in_state.gc) {
            printf("SDL could not connect controller");
            in_state.input_mode = KEYBOARD_MODE;
        }
    } else {
        in_state.input_mode = KEYBOARD_MODE;
    }
}

input_state_t* input_state_get() {
    return &in_state;
}

void input_state_zero() {
    for (int i = 0; i < INPUT_CNT; i++) {
        in_state.inputs[i] = false;
    }
    for (int i = 0; i < ANALOG_CNT; i++) {
        in_state.analog_inputs[i] = 0;
    }
}
