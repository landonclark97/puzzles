#ifndef __DATATHREADING__
#define __DATATHREADING__


#include <SDL2/SDL.h>
#include <flecs.h>


typedef ecs_os_thread_t puzzles_thread_t;

typedef ecs_os_mutex_t puzzles_mutex_t;

typedef ecs_os_cond_t puzzles_cond_t;

typedef ecs_os_thread_callback_t puzzles_thread_fn_t;


static ecs_os_thread_t puzzles_thread_new(ecs_os_thread_callback_t callback, void *arg) {

    SDL_Thread *thread;

    if (!(thread = SDL_CreateThread((SDL_ThreadFunction) callback, "ecs_thread", arg))) {
        abort();
    }

    return (ecs_os_thread_t)(uintptr_t)thread;
}

static void* puzzles_thread_join(ecs_os_thread_t thread) {
    int arg;
    SDL_Thread* thr = (SDL_Thread*) thread;
    SDL_WaitThread(thr, &arg);
    return 0;
}

static int32_t puzzles_ainc(int32_t *count) {
    SDL_AtomicIncRef((SDL_atomic_t*)count);
    return *count;
}

static int32_t puzzles_adec(int32_t *count) {
    SDL_AtomicDecRef((SDL_atomic_t*)count);
    return *count;
}

static ecs_os_mutex_t puzzles_mutex_new(void) {
    SDL_mutex* mutex;
    if (!(mutex = SDL_CreateMutex())) {
        abort();
    }
    return (ecs_os_mutex_t)(uintptr_t)mutex;
}

static void puzzles_mutex_free(ecs_os_mutex_t m) {
    SDL_mutex* mutex = (SDL_mutex*)(intptr_t)m;
    SDL_DestroyMutex(mutex);
}

static void puzzles_mutex_lock(ecs_os_mutex_t m) {
    SDL_mutex *mutex = (SDL_mutex*)(intptr_t)m;
    if (SDL_LockMutex(mutex)) {
        abort();
    }
}

static void puzzles_mutex_unlock(ecs_os_mutex_t m) {
    SDL_mutex *mutex = (SDL_mutex*)(intptr_t)m;
    if (SDL_UnlockMutex(mutex)) {
        abort();
    }
}

static ecs_os_cond_t puzzles_cond_new(void) {
    SDL_cond *cond;
    if (!(cond = SDL_CreateCond())) {
        abort();
    }
    return (ecs_os_cond_t)(uintptr_t)cond;
}

static void puzzles_cond_free(ecs_os_cond_t c) {
    SDL_cond *cond = (SDL_cond*)(intptr_t)c;
    SDL_DestroyCond(cond);
}

static void puzzles_cond_signal(ecs_os_cond_t c) {
    SDL_cond *cond = (SDL_cond*)(intptr_t)c;
    if (SDL_CondSignal(cond)) {
        abort();
    }
}

static void puzzles_cond_broadcast(ecs_os_cond_t c) {
    SDL_cond *cond = (SDL_cond*)(intptr_t)c;
    if (SDL_CondBroadcast(cond)) {
        abort();
    }
}

static void puzzles_cond_wait(ecs_os_cond_t c, ecs_os_mutex_t m) {
    SDL_cond *cond = (SDL_cond*)(intptr_t)c;
    SDL_mutex *mutex = (SDL_mutex*)(intptr_t)m;
    if (SDL_CondWait(cond, mutex)) {
        abort();
    }
}


#endif
