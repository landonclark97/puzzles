#ifndef __TIMER__
#define __TIMER__


#include <stdbool.h>

#define USEC_TO_SEC(a) (a/1000000.0)


typedef struct {
    bool _running;
    double last_tick, r_ticks, p_ticks, acc;
    union {
        double target_freq;
        double target_time;
    };
} ticker_t;


void ticker_init(ticker_t* t, double target_freq);

void ticker_init_w_time(ticker_t* t, double target_time);

float ticker_decr_acc(ticker_t* t);


#endif
