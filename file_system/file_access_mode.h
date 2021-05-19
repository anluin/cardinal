#pragma once

#include <stdbool.h>


typedef enum FileAccessMode {
    FileAccessMode_NONE           = 0x00,
    FileAccessMode_READ           = 0x01,
    FileAccessMode_WRITE          = 0x10,
    FileAccessMode_READ_AND_WRITE = FileAccessMode_READ | FileAccessMode_WRITE,
} file_access_mode_t;

__attribute__((overloadable))
void toggle(file_access_mode_t* self_ptr, file_access_mode_t flag, bool value);
