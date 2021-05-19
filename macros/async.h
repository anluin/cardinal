#pragma once


#define ASYNC(SELF_PTR, CONTEXT, ...) ({ \
    __auto_type __future_ptr      = (future_t*)SELF_PTR; \
    __auto_type __future_context  = CONTEXT; \
__loop: \
    for (;;) { \
        switch (self_ptr->state.idx) { \
            case 0: { \
             __VA_ARGS__;\
            } \
        }\
        return Poll_READY;\
    }\
}) \


#define __AWAIT(FUTURE_PTR, IDX) ({ \
    __future_ptr->sub_future_ptr = (FUTURE_PTR); \
    case IDX + 0:;\
    switch (poll(__future_ptr->sub_future_ptr, __future_context)) { \
        case Poll_PENDING: \
            self_ptr->state.idx = IDX + 0; \
            return Poll_PENDING; \
        case Poll_READY: \
            self_ptr->state.idx = IDX + 1; \
            goto __loop; \
    } \
    case IDX + 1:; \
}) \

#define AWAIT(FUTURE_PTR) __AWAIT(FUTURE_PTR, __LINE__);
