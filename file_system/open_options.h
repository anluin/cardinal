#pragma once

#include <stdbool.h>

#include "file_access_mode.h"


typedef struct OpenOptions {
    file_access_mode_t access_mode;
} open_options_t;

__attribute__((overloadable))
open_options_t OpenOptions(void);

__attribute__((overloadable))
void read(open_options_t* self, bool value);

__attribute__((overloadable))
open_options_t read(open_options_t self, bool value);

#include "file.h"

__attribute__((overloadable))
result_t(file_t, file_error_t) open(open_options_t self, const char* pathname);
