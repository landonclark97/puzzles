#include <stdio.h>

#include "./camera.h"


void camera_init(camera_t* cam, float mx, float my) {
    cam->x = 0.0;
    cam->y = 0.0;
    cam->target_x = 0.0;
    cam->target_y = 0.0;
    cam->middle_x = mx;
    cam->middle_y = my;
}

void camera_resize(camera_t* cam, float mx, float my) {
    cam->middle_x = mx;
    cam->middle_y = my;
}

void camera_set_target(camera_t* cam, float _x, float _y, double dt) {
    cam->target_x = _x;
    cam->target_y = _y;

    cam->x = cam->x + (LERP_FACTOR*(float)dt*(cam->target_x-cam->x));
    cam->y = cam->y + (LERP_FACTOR*(float)dt*(cam->target_y-cam->y));
}

void camera_frame(camera_t* cam, float* x, float* y) {
    *x = *x + cam->middle_x - cam->x;
    *y = *y + cam->middle_y - cam->y;
}
