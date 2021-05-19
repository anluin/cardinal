#pragma once


#define CLEANUP_FUNCTION(TYPE)   \
    cleanup_function_of_ ## TYPE \

#define MAKE_TIDY(TYPE)                           \
    __attribute__((overloadable))                 \
    void cleanup(TYPE* self_ptr);                 \
                                                  \
    __attribute__((overloadable, weak))           \
    void CLEANUP_FUNCTION(TYPE)(TYPE* self_ptr) { \
        cleanup(self_ptr);                        \
    }                                             \

#define TIDY(TYPE)                                   \
    __attribute__((cleanup(CLEANUP_FUNCTION(TYPE)))) \
    TYPE                                             \
