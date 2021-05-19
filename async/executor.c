#include <stdlib.h>
#include <stdio.h>

#include "executor.h"


__attribute__((overloadable))
executor_t Executor(void) {
    return (executor_t) {
        .reactor                 = Reactor(),
        .entries_length          = 0,
        .awaken_entry_ids_length = 0,
    };
}

__attribute__((overloadable))
void cleanup(executor_t* self_ptr) {
    for (unsigned int index = 0; index < self_ptr->entries_length; ++index) {
        future_t* future_ptr = self_ptr->entries[index].future_ptr;

        if (future_ptr != NULL) {
            cleanup(future_ptr);
        }
    }

    self_ptr->entries_length = 0;

    cleanup(&self_ptr->reactor);
}

__attribute__((overloadable))
void wake(executor_t* self_ptr, void* future_ptr) {
    for (unsigned int entry_id = 0; entry_id < self_ptr->entries_length; ++entry_id) {
        executor_entry_t* entry_ptr = &self_ptr->entries[entry_id];

        if (entry_ptr->future_ptr == future_ptr) {
            self_ptr->awaken_entry_ids[self_ptr->awaken_entry_ids_length++] = entry_id;

            return;
        }
    }

    fprintf(stderr, "[ERROR] Task not found: %p\n", future_ptr);
    exit(EXIT_FAILURE);
}

__attribute__((overloadable))
void spawn(executor_t* self_ptr, future_t* future_ptr) {
    unsigned int entry_id = self_ptr->entries_length++;

    self_ptr->entries[entry_id] = (executor_entry_t) {
        .future_ptr = future_ptr,
        .waker      = Waker(self_ptr, future_ptr),
    };

    self_ptr->awaken_entry_ids[self_ptr->awaken_entry_ids_length++] = entry_id;
}

__attribute__((overloadable))
void block(executor_t* self_ptr) {
    for (;;) {
        for (unsigned int index = 0; index < self_ptr->awaken_entry_ids_length; ++index) {
            unsigned int      entry_id   = self_ptr->awaken_entry_ids[index];
            executor_entry_t* entry_ptr  = &self_ptr->entries[entry_id];
            future_t*         future_ptr = entry_ptr->future_ptr;

            if (future_ptr != NULL) {
                switch (poll(future_ptr, Context(&self_ptr->reactor, &entry_ptr->waker))) {
                    case Poll_PENDING:
                        break;
                    case Poll_READY:
                        entry_ptr->future_ptr = NULL;
                        cleanup(future_ptr);
                        break;
                }
            }
        }

        if (self_ptr->awaken_entry_ids_length == 0) {
            break;
        }

        self_ptr->awaken_entry_ids_length = 0;

        if (self_ptr->reactor.num_pending_events != 0) {
            ignite(&self_ptr->reactor);
        }
    }
}
