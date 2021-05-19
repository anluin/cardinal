#include "executor.h"

#include "waker.h"


__attribute__((overloadable))
waker_t Waker(executor_t* executor_ptr, future_t* future_ptr) {
    return (waker_t) {
        .executor_ptr = executor_ptr,
        .future_ptr   = future_ptr,
    };
}

__attribute__((overloadable))
void cleanup(waker_t* self_ptr) {

}

__attribute__((overloadable))
void wake(waker_t* self_ptr) {
    wake(self_ptr->executor_ptr, self_ptr->future_ptr);
}
