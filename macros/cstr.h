#pragma once

#define CSTR(SAMPLE) ({                                     \
    __auto_type sample = CSTR_normalize(SAMPLE);            \
    __auto_type buffer_capacity = CSTR_length(&SAMPLE) + 1; \
    __auto_type buffer = __builtin_alloca(buffer_capacity); \
    CSTR_write(&sample, buffer, buffer_capacity);           \
    CSTR_cleanup(SAMPLE);                                   \
    (char*)buffer;                                          \
})                                                          \
