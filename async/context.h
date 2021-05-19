#pragma once


typedef struct Reactor reactor_t;
typedef struct Waker   waker_t;

typedef struct Context {
    struct Reactor* reactor_ptr;
    struct Waker*   waker_ptr;
} context_t;

__attribute__((overloadable))
context_t Context(reactor_t* reactor_ptr, waker_t* waker_ptr);

__attribute__((overloadable))
void cleanup(context_t* self_ptr);
