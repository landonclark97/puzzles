#include <sys/time.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "./ticker.h"


static inline double get_time() {
    struct timeval time;
    gettimeofday(&time, NULL);

    double sec = (double) time.tv_sec, usec = (double) time.tv_usec;
    usec = USEC_TO_SEC(usec);

    return sec+usec;
}

void ticker_init(ticker_t* t, double target_freq) {
    t->_running = true;
    t->last_tick = get_time();
    t->r_ticks = 0.0;
    t->p_ticks = 0.0;
    t->acc = 0.0;
    t->target_freq = 1.0/target_freq;
}

void ticker_init_w_time(ticker_t* t, double target_time) {
    t->_running = true;
    t->last_tick = get_time();
    t->r_ticks = 0.0;
    t->p_ticks = 0.0;
    t->acc = 0.0;
    t->target_time = target_time;
}

float ticker_decr_acc(ticker_t* t) {
    double tick = get_time();

    t->acc += tick - t->last_tick;
    t->last_tick = tick;

    double acc = 0.0;
    if (t->acc >= t->target_freq) {
        acc = t->acc;
        t->acc = 0.0;
        if (t->_running) {
            t->r_ticks += acc;
        } else {
            t->p_ticks += acc;
        }
    }
    return acc;
}

bool ticker_pause(ticker_t* t) {
    if (t->_running) {
        t->_running = false;
        return true;
    }
    return false;
}

bool ticker_resume(ticker_t* t) {
    if (!t->_running) {
        t->_running = true;
        t->p_ticks = 0.0;
        return true;
    }
    return false;
}
