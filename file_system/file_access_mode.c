#include "file_access_mode.h"


__attribute__((overloadable))
void toggle(file_access_mode_t* self_ptr, file_access_mode_t flag, bool value) {
    if (value) {
        *self_ptr |= flag;
    } else {
        *self_ptr &= ~flag;
    }
}
