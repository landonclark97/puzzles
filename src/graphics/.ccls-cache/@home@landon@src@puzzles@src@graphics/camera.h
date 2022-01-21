#ifndef __CAMERA__
#define __CAMERA__


#define LERP_FACTOR (2.0)


typedef struct {
    float x, y;
    float target_x, target_y;
    float middle_x, middle_y;
} camera_t;


void camera_init(camera_t* cam, float mx, float my);

void camera_resize(camera_t* cam, float mx, float my);

void camera_set_target(camera_t* cam, float _x, float _y, double dt);

void camera_frame(camera_t* cam, float* x, float* y);





#endif
