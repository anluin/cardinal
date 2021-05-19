#include <stdio.h>
#include <unistd.h>

#include "../../macros/box.h"

#include "../reactor.h"

#include "read_future.h"


__attribute__((overloadable))
read_future_t ReadFuture(int file_descriptor, uint8_t* buffer, size_t buffer_capacity, ssize_t* num_bytes_read_ptr) {
    return (read_future_t) {
        .future = FUTURE(read_future_t),
        .state_0 = (read_future_state_0_t) {
            .idx             = 0,
            .file_descriptor = file_descriptor,
            .buffer          = buffer,
            .buffer_capacity = buffer_capacity,
            .num_bytes_read_ptr  = num_bytes_read_ptr,
        },
    };
}

__attribute__((overloadable))
void cleanup(read_future_t* self_ptr) {
}

__attribute__((overloadable))
poll_t poll(read_future_t* self_ptr, context_t context) {
    for (;;) {
        switch (self_ptr->state.idx) {
            case 0:
                add_read(context.reactor_ptr, self_ptr->state.file_descriptor, context.waker_ptr);
                self_ptr->state.idx += 1;

                return Poll_PENDING;
            default:
                *self_ptr->state.num_bytes_read_ptr = read(
                    self_ptr->state.file_descriptor, 
                    self_ptr->state.buffer,
                    self_ptr->state.buffer_capacity
                );

                return Poll_READY;
        }
    }
}

__attribute__((overloadable))
future_t* read(int file_descriptor, uint8_t* buffer, size_t buffer_capacity, ssize_t* num_bytes_read_ptr) {
    /*
        Once a feature is spawned, it is not allowed to change its address.

        For the sake of simplicity, the memory is allocated here with "malloc".
                                                                                */
    future_t* future_ptr = (future_t*)BOX(ReadFuture(file_descriptor, buffer, buffer_capacity, num_bytes_read_ptr));

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
