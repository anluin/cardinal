#include <stdlib.h>
#include <stdio.h>

#include "future.h"


__attribute__((overloadable))
future_t Future(poll_fn_t poll_fn, cleanup_fn_t cleanup_fn) {
    return (future_t) {
        .poll_fn        = poll_fn,
        .cleanup_fn     = cleanup_fn,
        .deallocate_fn  = NULL,

        .sub_future_ptr = NULL,
    };
}

__attribute__((overloadable))
void cleanup(future_t* self_ptr) {
    if (self_ptr->cleanup_fn != NULL) {
        self_ptr->cleanup_fn(self_ptr);
    } else {
        fprintf(stderr, "[ERROR] Somethign went wrong!\n");
        exit(EXIT_FAILURE);
    }

    if (self_ptr->deallocate_fn != NULL) {
        self_ptr->deallocate_fn(self_ptr);
    }
}

__attribute__((overloadable))
poll_t poll(future_t* self_ptr, context_t context) {
    if (self_ptr->cleanup_fn != NULL) {
        return self_ptr->poll_fn(self_ptr, context);
    } else {
        fprintf(stderr, "[ERROR] Somethign went wrong!\n");
        exit(EXIT_FAILURE);
    }
}
