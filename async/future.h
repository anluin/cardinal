#pragma once

#include "context.h"


#define FUTURE(TYPE) Future(                         \
    (poll_fn_t)   (poll_t(*)(TYPE*, context_t))poll, \
    (cleanup_fn_t)(void  (*)(TYPE*))cleanup          \
)                                                    \

typedef enum Poll {
    Poll_PENDING,
    Poll_READY,
} poll_t;

typedef poll_t(*poll_fn_t)(void* self_ptr, context_t context);
typedef void(*cleanup_fn_t)(void* self_ptr);
typedef void(*deallocate_fn_t)(void* self_ptr);

typedef struct Future {
    poll_fn_t      poll_fn;
    cleanup_fn_t   cleanup_fn;
    deallocate_fn_t   deallocate_fn;

    struct Future* sub_future_ptr;
} future_t;

__attribute__((overloadable))
future_t Future(poll_fn_t poll_fn, cleanup_fn_t cleanup_fn);

__attribute__((overloadable))
void cleanup(future_t* self_ptr);

__attribute__((overloadable))
poll_t poll(future_t* self_ptr, context_t context);
