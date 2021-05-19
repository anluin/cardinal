#pragma once

#include <stdbool.h>

#include "../future.h"


typedef struct TimeoutFutureState_0 {
    unsigned int idx;
    unsigned long milliseconds;
} timeout_future_state_0_t;

typedef struct TimeoutFuture {
    struct Future future;

    union {
        struct {
            struct TimeoutFutureState_0;
        } state;

        struct {
            struct TimeoutFutureState_0 state_0;
        };
    };
} timeout_future_t;

__attribute__((overloadable))
timeout_future_t TimeoutFuture(unsigned long milliseconds);

__attribute__((overloadable))
void cleanup(timeout_future_t* self_ptr);

__attribute__((overloadable))
poll_t poll(timeout_future_t* self_ptr, context_t context);

__attribute__((overloadable))
future_t* timeout(unsigned long milliseconds);
