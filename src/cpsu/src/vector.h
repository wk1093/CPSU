#pragma once
#ifndef SRC_CPSU_VECTOR_H
#define SRC_CPSU_VECTOR_H

#include "../stuff/_cpsu_include.h"

BEGIN_CPSU_CODE()

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
        vec->data = realloc(vec->data, vec->capacity * sizeof(T));  \
        if (vec->data == NULL) { \
            fprintf(stderr, "suVectorAdd_" #vecext ": realloc failed\n"); \
            exit(1); \
        } \
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
        vec->data = realloc(vec->data, vec->capacity * sizeof(T));  \
        if (vec->data == NULL) { \
            fprintf(stderr, "suVectorInsert_" #vecext ": realloc failed\n"); \
            exit(1); \
        } \
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
        vec->data = realloc(vec->data, vec->capacity * sizeof(T));  \
        if (vec->data == NULL) { \
            fprintf(stderr, "suVectorResize_" #vecext ": realloc failed\n"); \
            exit(1); \
        } \
    } \
} \
void suVectorReserve_##vecext(suVector_##vecext *vec, size_t capacity) { \
    if (vec->capacity < capacity) { \
        vec->capacity = capacity; \
        vec->data = realloc(vec->data, vec->capacity * sizeof(T));  \
        if (vec->data == NULL) { \
            fprintf(stderr, "suVectorReserve_" #vecext ": realloc failed\n"); \
            exit(1); \
        } \
    } \
}

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

END_CPSU_CODE()





#endif //SRC_CPSU_VECTOR_H
