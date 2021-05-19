#include <stdlib.h>

#include "file_error.h"


__attribute__((overloadable))
file_error_t FileError(char* message) {
    return (file_error_t) {
        .message = message,
    };
}

__attribute__((overloadable))
void cleanup(file_error_t* self_ptr) {
    free(self_ptr->message);
}
