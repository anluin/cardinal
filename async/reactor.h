#pragma once

#include "waker.h"


typedef struct Reactor {
    int handle;
    int num_pending_events;
} reactor_t;

__attribute__((overloadable))
reactor_t Reactor(void);

__attribute__((overloadable))
void cleanup(reactor_t* self_ptr);

__attribute__((overloadable))
void add_timeout(reactor_t* self_ptr, unsigned long milliseconds, waker_t* waker_ptr);

__attribute__((overloadable))
void add_read(reactor_t* self_ptr, int file_descriptor, waker_t* waker_ptr);

__attribute__((overloadable))
void ignite(reactor_t* self_ptr);
