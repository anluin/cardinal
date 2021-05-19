#pragma once

#include <stdlib.h>


#define BOX(...) ({                                   \
    __auto_type __box_instance = (__VA_ARGS__);       \
    __typeof__(__box_instance)* __box_instance_ptr    \
        = malloc(sizeof(__typeof__(__box_instance))); \
    *__box_instance_ptr = __box_instance;             \
    __box_instance_ptr;                               \
})                                                    \
