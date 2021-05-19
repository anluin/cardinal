#pragma once


typedef enum ResultKind {
    ResultKind_OK,
    ResultKind_ERROR,
} result_kind_t;

#define __Result(OK_TYPE, ERROR_TYPE) \
    ResultOf_ ## OK_TYPE ## _and_ ## ERROR_TYPE
#define Result(...) __Result(__VA_ARGS__)

#define __result_t(OK_TYPE, ERROR_TYPE) \
    result_of_ ## OK_TYPE ## _and_ ## ERROR_TYPE ## _t
#define result_t(...) __result_t(__VA_ARGS__)

#define __Ok(OK_TYPE, ERROR_TYPE) \
    OkOf_ ## OK_TYPE ## _and_ ## ERROR_TYPE
#define Ok(...) __Ok(__VA_ARGS__)

#define __Error(OK_TYPE, ERROR_TYPE) \
    ErrorOf_ ## OK_TYPE ## _and_ ## ERROR_TYPE
#define Error(...) __Error(__VA_ARGS__)

#define RESULT_TYPE(OK_TYPE, ERROR_TYPE)                                         \
    typedef struct Result(OK_TYPE, ERROR_TYPE) {                                 \
        enum ResultKind kind;                                                    \
        union {                                                                  \
            OK_TYPE ok;                                                          \
            ERROR_TYPE error;                                                    \
        } data;                                                                  \
    } result_t(OK_TYPE, ERROR_TYPE);                                             \
                                                                                 \
    __attribute__((overloadable, always_inline, weak, warn_unused_result))       \
    result_t(OK_TYPE, ERROR_TYPE) Ok(OK_TYPE, ERROR_TYPE)(OK_TYPE value) {       \
        return (result_t(OK_TYPE, ERROR_TYPE)) {                                 \
            .kind = ResultKind_OK,                                               \
            .data.ok = value,                                                    \
        };                                                                       \
    }                                                                            \
                                                                                 \
    __attribute__((overloadable, always_inline, weak, warn_unused_result))       \
    result_t(OK_TYPE, ERROR_TYPE) Error(OK_TYPE, ERROR_TYPE)(ERROR_TYPE value) { \
        return (result_t(OK_TYPE, ERROR_TYPE)) {                                 \
            .kind = ResultKind_ERROR,                                            \
            .data.error = value,                                                 \
        };                                                                       \
    }                                                                            \
                                                                                 \
    __attribute__((overloadable))                                                \
    void cleanup(OK_TYPE* self_ptr);                                             \
                                                                                 \
    __attribute__((overloadable))                                                \
    void cleanup(ERROR_TYPE* self_ptr);                                          \
                                                                                 \
    __attribute__((overloadable, always_inline, weak))                           \
    void cleanup(result_t(OK_TYPE, ERROR_TYPE)* self_ptr) {                      \
        switch (self_ptr->kind) {                                                \
            case ResultKind_OK:                                                  \
                cleanup(&self_ptr->data.ok);                                     \
                break;                                                           \
            case ResultKind_ERROR:                                               \
                cleanup(&self_ptr->data.error);                                  \
                break;                                                           \
        }                                                                        \
    }                                                                            \
                                                                                 \
    __attribute__((overloadable, always_inline, weak, warn_unused_result))       \
    unsigned char is_ok(result_t(OK_TYPE, ERROR_TYPE)* self_ptr) {               \
        return self_ptr->kind == ResultKind_OK;                                  \
    }                                                                            \
                                                                                 \
    __attribute__((overloadable, always_inline, weak, warn_unused_result))       \
    unsigned char is_error(result_t(OK_TYPE, ERROR_TYPE)* self_ptr) {            \
        return self_ptr->kind == ResultKind_ERROR;                               \
    }                                                                            \
                                                                                 \
    __attribute__((overloadable, always_inline, weak, warn_unused_result))       \
    OK_TYPE unwrap(result_t(OK_TYPE, ERROR_TYPE) self) {                         \
        return self.data.ok;                                                     \
    }                                                                            \
                                                                                 \
    __attribute__((overloadable, always_inline, weak, warn_unused_result))       \
    ERROR_TYPE unwrap_error(result_t(OK_TYPE, ERROR_TYPE) self) {                \
        return self.data.error;                                                  \
    }                                                                            \

#define TRY(RESULT, ...)               \
    unwrap(({                          \
        __auto_type result = (RESULT); \
                                       \
        if (is_error(&result)) {       \
            __auto_type ERROR =        \
                unwrap_error(result);  \
            return (__VA_ARGS__);      \
        }                              \
                                       \
        result;                        \
    }))                                \

#define TRY_DROP_ERROR(RESULT, ...)    \
    unwrap(({                          \
        __auto_type result = (RESULT); \
                                       \
        if (is_error(&result)) {       \
            cleanup(&result);          \
            return (__VA_ARGS__);      \
        }                              \
                                       \
        result;                        \
    }))                                \
