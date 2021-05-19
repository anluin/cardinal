#pragma once

#include "../macros/tidy.h"

#include "waker.h"
#include "reactor.h"
#include "future.h"


typedef struct ExecutorEntry {
    struct Waker   waker;
    struct Future* future_ptr;
    struct Future* sub_future_ptr;
} executor_entry_t;

typedef struct Executor {
    struct Reactor       reactor;

    struct ExecutorEntry entries[32];
    unsigned int         entries_length;

    unsigned int         awaken_entry_ids[32];
    unsigned int         awaken_entry_ids_length;
} executor_t;

MAKE_TIDY(executor_t);

__attribute__((overloadable))
executor_t Executor(void);

__attribute__((overloadable))
void cleanup(executor_t* self_ptr);

__attribute__((overloadable))
void wake(executor_t* self_ptr, void* future_ptr);

__attribute__((overloadable))
void spawn(executor_t* self_ptr, future_t* future_ptr);

__attribute__((overloadable))
void block(executor_t* self_ptr);
