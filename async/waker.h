#pragma once


typedef struct Executor executor_t;
typedef struct Future future_t;

typedef struct Waker {
    struct Executor* executor_ptr;
    struct Future*   future_ptr;
} waker_t;

__attribute__((overloadable))
waker_t Waker(executor_t* executor_ptr, future_t* future_ptr);

__attribute__((overloadable))
void cleanup(waker_t* self_ptr);

__attribute__((overloadable))
void wake(waker_t* self_ptr);
