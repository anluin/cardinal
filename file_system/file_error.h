#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


typedef struct FileError {
    char* message;
} file_error_t;

__attribute__((overloadable))
file_error_t FileError(char* message);

__attribute__((overloadable))
void cleanup(file_error_t* self_ptr);


// Implement CSTR-Macro interface
__attribute__((overloadable, always_inline, weak))
file_error_t CSTR_normalize(file_error_t* self_ptr) {
    return *self_ptr;
}

__attribute__((overloadable, always_inline, weak))
file_error_t CSTR_normalize(file_error_t self) {
    return self;
}

__attribute__((overloadable, always_inline, weak))
size_t CSTR_length(file_error_t* self_ptr) {
    return strlen(self_ptr->message);
}

__attribute__((overloadable, always_inline, weak))
void CSTR_write(file_error_t* self_ptr, char* buffer, size_t buffer_capacity) {
    strncpy(buffer, self_ptr->message, buffer_capacity);
}

__attribute__((overloadable, always_inline, weak))
void CSTR_cleanup(file_error_t* self_ptr) {}

__attribute__((overloadable, always_inline, weak))
void CSTR_cleanup(file_error_t self) {
    cleanup(&self);
}
