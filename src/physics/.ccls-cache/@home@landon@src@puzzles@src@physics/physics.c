#include <chipmunk/chipmunk.h>
#include <chipmunk/cpHastySpace.h>

#include <stdlib.h>
#include <stdio.h>

#include "./physics.h"


static phys_world_t* p_world;

void phys_world_init(phys_vec_t g) {
    p_world = cpHastySpaceNew();
    cpHastySpaceSetThreads(p_world, 2);
    cpSpaceSetGravity(p_world, g);
    cpSpaceSetIterations(p_world, 10);
}

void phys_world_step(double dt) {
    cpHastySpaceStep(p_world, dt);
}

void phys_world_pre_collision_callback(void* func, uint32_t typea, uint32_t typeb) {
    cpCollisionHandler* h = cpSpaceAddCollisionHandler(p_world, typea, typeb);
    h->preSolveFunc = func;
}

void phys_world_collision_callback(void* func, uint32_t typea, uint32_t typeb) {
    cpCollisionHandler* h = cpSpaceAddCollisionHandler(p_world, typea, typeb);
    h->beginFunc = func;
}

void phys_world_post_collision_callback(void* func, uint32_t typea, uint32_t typeb) {
    cpCollisionHandler* h = cpSpaceAddCollisionHandler(p_world, typea, typeb);
    h->postSolveFunc = func;
}

void phys_bc_set_circle(phys_body_constructor_t* bc, float mass, float radius, float _inner_rad, phys_vec_t offset, float x, float y, uint32_t catg, uint32_t mask) {
    bc->type = CIRCLE;
    bc->mass = mass;
    bc->radius = radius;
    bc->_inner = _inner_rad;
    bc->offset = offset;
    bc->pos_x = x;
    bc->pos_y = y;
    bc->catg = catg;
    bc->mask = mask;
}

void phys_bc_set_box(phys_body_constructor_t* bc, float mass, float width, float height, float _bevel, float x, float y, uint32_t catg, uint32_t mask) {
    bc->type = BOX;
    bc->mass = mass;
    bc->width = width;
    bc->height = height;
    bc->_bevel = _bevel;
    bc->pos_x = x;
    bc->pos_y = y;
    bc->catg = catg;
    bc->mask = mask;
}

void phys_bc_set_line(phys_body_constructor_t* bc, float mass, phys_vec_t* verts, float radius, float x, float y, uint32_t catg, uint32_t mask) {
    bc->type = LINE;
    bc->mass = mass;
    bc->radius = radius;
    bc->verts = verts;
    bc->pos_x = x;
    bc->pos_y = y;
    bc->catg = catg;
    bc->mask = mask;
}

void phys_bc_set_poly(phys_body_constructor_t* bc, float mass, int c, phys_vec_t* verts, phys_vec_t offset, float radius, float _bevel, float x, float y, uint32_t catg, uint32_t mask) {
    bc->type = POLYGON;
    bc->mass = mass;
    bc->count = c;
    bc->verts = verts;
    bc->offset = offset;
    bc->radius = radius;
    bc->_bevel = _bevel;
    bc->pos_x = x;
    bc->pos_y = y;
    bc->catg = catg;
    bc->mask = mask;
}

phys_body_t phys_body_new(phys_body_constructor_t* bc) {
    phys_body_t body;

    float moment;
    float mass = bc->mass;

    cpShapeFilter f;

    switch (bc->type) {
        case CIRCLE:
            moment = cpMomentForCircle(mass, bc->_inner, bc->radius, bc->offset);
            body.body = cpSpaceAddBody(p_world, cpBodyNew(mass, moment));
            cpBodySetPosition(body.body, (phys_vec_t) {bc->pos_x,bc->pos_y});
            body.shape = cpSpaceAddShape(p_world, cpCircleShapeNew(body.body, bc->radius, bc->offset));
            f.categories = bc->catg;
            f.mask = bc->mask;
            cpShapeSetFilter(body.shape, f);
            cpShapeSetCollisionType(body.shape, bc->catg);
            break;
        case BOX:
            moment = cpMomentForBox(mass, bc->width, bc->height);
            body.body = cpSpaceAddBody(p_world, cpBodyNew(mass, moment));
            cpBodySetPosition(body.body, (phys_vec_t) {bc->pos_x,bc->pos_y});
            body.shape = cpSpaceAddShape(p_world, cpBoxShapeNew(body.body, bc->width, bc->height, bc->_bevel));
            f.categories = bc->catg;
            f.mask = bc->mask;
            cpShapeSetFilter(body.shape, f);
            cpShapeSetCollisionType(body.shape, bc->catg);
            break;
        case LINE:
            moment = cpMomentForSegment(mass, bc->verts[0], bc->verts[1], bc->radius);
            body.body = cpSpaceAddBody(p_world, cpBodyNew(mass, moment));
            cpBodySetPosition(body.body, (phys_vec_t) {bc->pos_x,bc->pos_y});
            body.shape = cpSpaceAddShape(p_world, cpSegmentShapeNew(body.body, bc->verts[0], bc->verts[1], bc->radius));
            f.categories = bc->catg;
            f.mask = bc->mask;
            cpShapeSetFilter(body.shape, f);
            cpShapeSetCollisionType(body.shape, bc->catg);
            break;
        case POLYGON:
            moment = cpMomentForPoly(mass, bc->count, bc->verts, bc->offset, bc->_bevel);
            body.body = cpSpaceAddBody(p_world, cpBodyNew(mass, moment));
            cpBodySetPosition(body.body, (phys_vec_t) {bc->pos_x,bc->pos_y});
            body.shape = cpSpaceAddShape(p_world, cpPolyShapeNew(body.body, bc->count, bc->verts, (cpTransform) {1.0,0.0,0.0,1.0,0.0,0.0}, bc->radius));
            f.categories = bc->catg;
            f.mask = bc->mask;
            cpShapeSetFilter(body.shape, f);
            cpShapeSetCollisionType(body.shape, bc->catg);
            break;
    }

    return body;
}

phys_body_t phys_control_body_new() {
    return (phys_body_t) {cpSpaceAddBody(p_world, cpBodyNewKinematic()), NULL};
}

phys_vec_t phys_body_get_position(phys_body_t* body) {
    return cpBodyGetPosition(body->body);
}

void phys_body_set_position(phys_body_t* body, phys_vec_t v) {
    cpBodySetPosition(body->body, v);
}

double phys_body_get_angle(phys_body_t* body) {
    return cpBodyGetAngle(body->body);
}

void phys_body_set_angle(phys_body_t* body, double angle) {
    return cpBodySetAngle(body->body, angle);
}

phys_vec_t phys_body_get_velocity(phys_body_t* body) {
    return cpBodyGetVelocity(body->body);
}

void phys_body_set_velocity(phys_body_t* body, phys_vec_t v) {
    cpBodySetVelocity(body->body, v);
}

void phys_body_apply_force(phys_body_t* body, phys_vec_t force) {
    cpBodySetForce(body->body, force);
}

phys_constraint_t* phys_pivot_new(phys_body_t* a, phys_body_t* b, phys_vec_t p) {
    phys_constraint_t* piv = cpPivotJointNew(a->body, b->body, p);
    cpSpaceAddConstraint(p_world, piv);
    cpConstraintSetMaxBias(piv, 0.0);
    return piv;
}

void phys_constraint_set_force(phys_constraint_t* c, double f) {
    cpConstraintSetMaxForce(c, f);
}

void phys_arbiter_get_body(phys_arbiter_t* ab, phys_body_t* a, phys_body_t* b) {
    cpArbiterGetBodies(ab, &a->body, &b->body);
}

void phys_constraint_free(phys_constraint_t* c) {
    cpSpaceRemoveConstraint(p_world, c);
    cpConstraintFree(c);
}

void phys_body_free(phys_body_t* body) {
    cpSpaceRemoveBody(p_world, body->body);
    cpBodyFree(body->body);
    if (body->shape) {
        cpSpaceRemoveShape(p_world, body->shape);
        cpShapeFree(body->shape);
    }
}

void phys_world_free() {
    cpHastySpaceFree(p_world);
    p_world = NULL;
}
