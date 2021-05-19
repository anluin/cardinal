#include <stdlib.h>

#include "context.h"


__attribute__((overloadable))
context_t Context(reactor_t* reactor_ptr, waker_t* waker_ptr) {
    return (context_t) {
        .reactor_ptr        = reactor_ptr,
        .waker_ptr          = waker_ptr,
    };
}

__attribute__((overloadable))
void cleanup(context_t* self_ptr) {
    
}
