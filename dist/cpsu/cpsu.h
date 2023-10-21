#ifndef SRC_CPSU_CPSU_H
#define SRC_CPSU_CPSU_H

/**** start inlining _cpsu_include.h ****/
#ifndef SRC_CPSU__CPSU_INCLUDE_H
#define SRC_CPSU__CPSU_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef __bool_true_false_are_defined
// If for some reason bool is not defined, define it.
typedef enum suBool {
    suFalse = 0,
    suTrue = 1
} suBool;
#else
typedef bool suBool;
#define suFalse false
#define suTrue true
#endif

// in the source of cpsu, we want to be very explicit about what types we use, so using int, long, etc. is not allowed.
// instead, we use the types defined in stdint.h, which are guaranteed to be the same size on all platforms.
// this is important because we want to be able to use cpsu on embedded systems, where the size of int, long, etc. is not guaranteed.
// this is enforced by this:
#define int int_is_not_allowed
#define long long_is_not_allowed
#define short short_is_not_allowed
// #define char char_is_not_allowed // char is allowed because it is used for strings
#define signed signed_is_not_allowed
#define unsigned unsigned_is_not_allowed

#endif //SRC_CPSU__CPSU_INCLUDE_H
/**** ended inlining _cpsu_include.h ****/

/**** start inlining vector.h ****/
#ifndef SRC_CPSU_VECTOR_H
#define SRC_CPSU_VECTOR_H

/**** skipping file: _cpsu_include.h ****/

#define ___suVectorDeclare(T, vecext) \
typedef struct { \
    T *data; \
    size_t size; \
    size_t capacity; \
} suVector_##vecext; \
suVector_##vecext *suVectorNew_##vecext(); \
void suVectorFree_##vecext(suVector_##vecext *vec); \
void suVectorAdd_##vecext(suVector_##vecext *vec, T value); \
T suVectorGet_##vecext(suVector_##vecext *vec, size_t index); \
void suVectorSet_##vecext(suVector_##vecext *vec, size_t index, T value); \
void suVectorRemove_##vecext(suVector_##vecext *vec, size_t index); \
void suVectorInsert_##vecext(suVector_##vecext *vec, size_t index, T value); \
void suVectorClear_##vecext(suVector_##vecext *vec); \
void suVectorResize_##vecext(suVector_##vecext *vec, size_t size); \
void suVectorReserve_##vecext(suVector_##vecext *vec, size_t capacity);

___suVectorDeclare(int8_t, i8)
___suVectorDeclare(uint8_t, u8)
___suVectorDeclare(int16_t, i16)
___suVectorDeclare(uint16_t, u16)
___suVectorDeclare(int32_t, i32)
___suVectorDeclare(uint32_t, u32)
___suVectorDeclare(int64_t, i64)
___suVectorDeclare(uint64_t, u64)
___suVectorDeclare(float, f)
___suVectorDeclare(double, d)
___suVectorDeclare(char*, str)


#undef ___suVectorDeclare







#endif //SRC_CPSU_VECTOR_H
/**** ended inlining vector.h ****/

#ifdef CPSU_IMPLEMENTATION

/**** start inlining vector.c ****/
/**** skipping file: vector.h ****/

#define ___suVectorDefine(T, vecext) \
suVector_##vecext *suVectorNew_##vecext() { \
    suVector_##vecext *vec = malloc(sizeof(suVector_##vecext)); \
    vec->data = NULL; \
    vec->size = 0; \
    vec->capacity = 0; \
    return vec; \
} \
void suVectorFree_##vecext(suVector_##vecext *vec) { \
    free(vec->data); \
    free(vec); \
} \
void suVectorAdd_##vecext(suVector_##vecext *vec, T value) { \
    if (vec->size == vec->capacity) { \
        vec->capacity = vec->capacity == 0 ? 1 : vec->capacity * 2; \
        vec->data = realloc(vec->data, vec->capacity * sizeof(T)); \
    } \
    vec->data[vec->size++] = value; \
} \
T suVectorGet_##vecext(suVector_##vecext *vec, size_t index) { \
    return vec->data[index]; \
} \
void suVectorSet_##vecext(suVector_##vecext *vec, size_t index, T value) { \
    vec->data[index] = value; \
} \
void suVectorRemove_##vecext(suVector_##vecext *vec, size_t index) { \
    for (size_t i = index; i < vec->size - 1; i++) { \
        vec->data[i] = vec->data[i + 1]; \
    } \
    vec->size--; \
} \
void suVectorInsert_##vecext(suVector_##vecext *vec, size_t index, T value) { \
    if (vec->size == vec->capacity) { \
        vec->capacity = vec->capacity == 0 ? 1 : vec->capacity * 2; \
        vec->data = realloc(vec->data, vec->capacity * sizeof(T)); \
    } \
    for (size_t i = vec->size; i > index; i--) { \
        vec->data[i] = vec->data[i - 1]; \
    } \
    vec->data[index] = value; \
    vec->size++; \
} \
void suVectorClear_##vecext(suVector_##vecext *vec) { \
    vec->size = 0; \
} \
void suVectorResize_##vecext(suVector_##vecext *vec, size_t size) { \
    vec->size = size; \
    if (vec->capacity < size) { \
        vec->capacity = size; \
        vec->data = realloc(vec->data, vec->capacity * sizeof(T)); \
    } \
} \
void suVectorReserve_##vecext(suVector_##vecext *vec, size_t capacity) { \
    if (vec->capacity < capacity) { \
        vec->capacity = capacity; \
        vec->data = realloc(vec->data, vec->capacity * sizeof(T)); \
    } \
}

___suVectorDefine(int8_t, i8)
___suVectorDefine(uint8_t, u8)
___suVectorDefine(int16_t, i16)
___suVectorDefine(uint16_t, u16)
___suVectorDefine(int32_t, i32)
___suVectorDefine(uint32_t, u32)
___suVectorDefine(int64_t, i64)
___suVectorDefine(uint64_t, u64)
___suVectorDefine(float, f)
___suVectorDefine(double, d)
___suVectorDefine(char*, str)


#undef ___suVectorDefine
/**** ended inlining vector.c ****/

#endif //CPSU_IMPLEMENTATION

/**** start inlining _cpsu_end.h ****/
#ifndef SRC_CPSU__CPSU_END_H
#define SRC_CPSU__CPSU_END_H

#undef int
#undef long
#undef short
#undef char
#undef signed
#undef unsigned


#endif //SRC_CPSU__CPSU_END_H
/**** ended inlining _cpsu_end.h ****/

#endif //SRC_CPSU_CPSU_H
