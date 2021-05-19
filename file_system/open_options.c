#include "open_options.h"


__attribute__((overloadable))
open_options_t OpenOptions(void) {
    return (open_options_t) {
        .access_mode = FileAccessMode_NONE,
    };
}

__attribute__((overloadable))
void read(open_options_t* self, bool value) {
    toggle(&self->access_mode, FileAccessMode_READ, value);
}

__attribute__((overloadable))
open_options_t read(open_options_t self, bool value) {
    read(&self, value);

    return self;
}

__attribute__((overloadable))
result_t(file_t, file_error_t) open(open_options_t self, const char* pathname) {
    return File(pathname, self.access_mode);
}
