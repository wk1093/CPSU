#pragma once
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
