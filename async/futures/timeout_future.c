#include <stdio.h>

#include "../../macros/box.h"

#include "../reactor.h"

#include "timeout_future.h"


__attribute__((overloadable))
timeout_future_t TimeoutFuture(unsigned long milliseconds) {
    return (timeout_future_t) {
        .future = FUTURE(timeout_future_t),
        .state_0 = (timeout_future_state_0_t) {
            .idx          = 0,
            .milliseconds = milliseconds,
        },
    };
}

__attribute__((overloadable))
void cleanup(timeout_future_t* self_ptr) {
}

__attribute__((overloadable))
poll_t poll(timeout_future_t* self_ptr, context_t context) {
    for (;;) {
        switch (self_ptr->state.idx) {
            case 0:
                add_timeout(context.reactor_ptr, self_ptr->state.milliseconds, context.waker_ptr);
                self_ptr->state.idx += 1;

                return Poll_PENDING;
            default:
                return Poll_READY;
        }
    }
}

__attribute__((overloadable))
future_t* timeout(unsigned long milliseconds) {
    /*
        Once a feature is spawned, it is not allowed to change its address.

        For the sake of simplicity, the memory is allocated here with "malloc".
                                                                                */
    future_t* future_ptr = (future_t*)BOX(TimeoutFuture(milliseconds));
    
    /*
        The deallocate function is called after cleanup.
        Serves to be able to free up allocated memory.

        Since the memory was here allocated via "malloc",
        we can simply use "free" here.

        (if "deallocate_fn" is "null", it will not be called.)
                                                                */
    future_ptr->deallocate_fn = free;

    return future_ptr;
}
