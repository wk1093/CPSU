#pragma once
#ifndef SRC_CPSU_VECTOR_H
#define SRC_CPSU_VECTOR_H

#include "_cpsu_include.h"

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
