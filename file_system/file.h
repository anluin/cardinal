#pragma once

#include "../macros/tidy.h"
#include "../macros/result.h"

#include "file_access_mode.h"


typedef struct File {
    int handle;
    file_access_mode_t access_mode;
} file_t;

MAKE_TIDY(file_t);

#include "file_error.h"

RESULT_TYPE(file_t, file_error_t);

__attribute__((overloadable))
result_t(file_t, file_error_t) File(const char* pathname, file_access_mode_t access_mode);

__attribute__((overloadable))
void cleanup(file_t* self_ptr);

__attribute__((overloadable))
size_t get_size(file_t* self_ptr);
