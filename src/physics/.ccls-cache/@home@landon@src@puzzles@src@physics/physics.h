#ifndef __PHYSWORLD__
#define __PHYSWORLD__

#include <chipmunk/chipmunk.h>


enum {
CIRCLE,
BOX,
ELLIPSE,
LINE,
POLYGON
};


typedef cpSpace phys_world_t;

typedef struct {
    cpBody* body;
    cpShape* shape;
} phys_body_t;

typedef cpVect phys_vec_t;

typedef struct {
    int type;
    float mass;
    float pos_x, pos_y;
    int count;
    union {
        struct {
            float width, height;
        };
        struct {
            float radius;
            float _;
        };
    };
    union {
        float _inner;
        float _bevel;
    };
    phys_vec_t* verts;
    phys_vec_t offset;
    uint32_t catg;
    uint32_t mask;
} phys_body_constructor_t;

typedef cpConstraint phys_constraint_t;

typedef cpArbiter phys_arbiter_t;


void phys_world_init(phys_vec_t g);

void phys_world_step(double dt);

void phys_world_pre_collision_callback(void* func, uint32_t typea, uint32_t typeb);

void phys_world_collision_callback(void* func, uint32_t typea, uint32_t typeb);

void phys_world_post_collision_callback(void* func, uint32_t typea, uint32_t typeb);

void phys_bc_set_circle(phys_body_constructor_t* bc, float mass, float radius, float _inner_rad, phys_vec_t offset, float x, float y, uint32_t catg, uint32_t mask);

void phys_bc_set_box(phys_body_constructor_t* bc, float mass, float width, float height, float _bevel, float x, float y, uint32_t catg, uint32_t mask);

void phys_bc_set_line(phys_body_constructor_t* bc, float mass, phys_vec_t* verts, float radius, float x, float y, uint32_t catg, uint32_t mask);

void phys_bc_set_poly(phys_body_constructor_t* bc, float mass, int c, phys_vec_t* verts, phys_vec_t offset, float radius, float _bevel, float x, float y, uint32_t catg, uint32_t mask);

phys_body_t phys_body_new(phys_body_constructor_t* bc);

phys_body_t phys_control_body_new();

phys_vec_t phys_body_get_position(phys_body_t* body);

void phys_body_set_position(phys_body_t* body, phys_vec_t v);

double phys_body_get_angle(phys_body_t* body);

void phys_body_set_angle(phys_body_t* body, double angle);

phys_vec_t phys_body_get_velocity(phys_body_t* body);

void phys_body_set_velocity(phys_body_t* body, phys_vec_t v);

void phys_body_apply_force(phys_body_t* body, phys_vec_t force);

phys_constraint_t* phys_pivot_new(phys_body_t* a, phys_body_t* b, phys_vec_t p);

void phys_constraint_set_force(phys_constraint_t* c, double f);

void phys_constraint_free(phys_constraint_t* c);

void phys_arbiter_get_body(phys_arbiter_t* ab, phys_body_t* a, phys_body_t* b);

void phys_body_free(phys_body_t* body);

void phys_world_free();




#endif
