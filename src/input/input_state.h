#ifndef __INPUTSTATE__
#define __INPUTSTATE__

#include <SDL2/SDL.h>


#define MAX(a, b) ((a > b) ? a : b)

#define INPUT_CNT (21)
#define ANALOG_CNT (8)

#define GC_DEADZONE (6000)

enum {
JOYSTICK_MODE,
KEYBOARD_MODE
};

enum {
W,
A,
S,
D,
Q,
E,
R,
F,
T,
Z,
X,
C,
LSHIFT,
LCTRL,
LALT,
SPACE,
RETURN,
NOKEY,
};


typedef struct {
    char input_mode;
    char inputs[MAX(NOKEY,INPUT_CNT)];
    union {
        int16_t analog_inputs[ANALOG_CNT];
        struct {
            uint16_t mouse_x, mouse_y;
            uint16_t _[ANALOG_CNT-2];
        };
    };
    int16_t gc_deadzone;
    SDL_GameController* gc;
} input_state_t;


void input_state_init();

input_state_t* input_state_get();

void input_state_zero();


#endif
