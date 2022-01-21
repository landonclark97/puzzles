#include <flecs.h>

#include <stdlib.h>
#include <stdbool.h>

#include "./data_handler.h"

#include "../common/threading.h"


static data_world_t* d_world;


void data_world_set_os_api();


data_world_t* data_world() {
    return d_world;
}

void data_world_init() {
    data_world_set_os_api();
    d_world = ecs_init();
    ecs_set_threads(d_world, DATA_THREADS);
}

bool data_world_step(double dt) {
    return ecs_progress(d_world, (float) dt);
}

void data_world_free() {
    ecs_fini(d_world);
    d_world = NULL;
}

void data_world_set_os_api() {
    ecs_os_set_api_defaults();

    ecs_os_api_t api = ecs_os_api;

    api.thread_new_ = puzzles_thread_new;
    api.thread_join_ = puzzles_thread_join;
    api.ainc_ = puzzles_ainc;
    api.adec_ = puzzles_adec;
    api.mutex_new_ = puzzles_mutex_new;
    api.mutex_free_ = puzzles_mutex_free;
    api.mutex_lock_ = puzzles_mutex_lock;
    api.mutex_unlock_ = puzzles_mutex_unlock;
    api.cond_new_ = puzzles_cond_new;
    api.cond_free_ = puzzles_cond_free;
    api.cond_signal_ = puzzles_cond_signal;
    api.cond_broadcast_ = puzzles_cond_broadcast;
    api.cond_wait_ = puzzles_cond_wait;

    ecs_os_set_api(&api);
}
