#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>

#include "../future.h"


typedef struct ReadFutureState_0 {
    unsigned int idx;

    int          file_descriptor;
    uint8_t*     buffer;
    size_t       buffer_capacity;
    ssize_t*     num_bytes_read_ptr;
} read_future_state_0_t;

typedef struct ReadFuture {
    struct Future future;

    union {
        struct {
            struct ReadFutureState_0;
        } state;

        struct {
            struct ReadFutureState_0 state_0;
        };
    };
} read_future_t;

__attribute__((overloadable))
read_future_t ReadFuture(int file_descriptor, uint8_t* buffer, size_t buffer_capacity, ssize_t* num_bytes_read_ptr);

__attribute__((overloadable))
void cleanup(read_future_t* self_ptr);

__attribute__((overloadable))
poll_t poll(read_future_t* self_ptr, context_t context);

__attribute__((overloadable))
future_t* read(int file_descriptor, uint8_t* buffer, size_t buffer_capacity, ssize_t* num_bytes_read_ptr);
