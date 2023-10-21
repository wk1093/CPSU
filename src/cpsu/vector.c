#include "vector.h"

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
