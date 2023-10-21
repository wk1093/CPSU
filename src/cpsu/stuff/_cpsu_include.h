#pragma once
#ifndef SRC_CPSU__CPSU_INCLUDE_H
#define SRC_CPSU__CPSU_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// mostly filesystem headers
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#define SU_PLATFORM_WINDOWS
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define SU_PLATFORM_UNIX
#endif

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

// if we are on an x86_64 system, define SU_BITS_64
// if we are on an x86 system, define SU_BITS_32
// else, define SU_BITS_UNKNOWN
#if defined(__x86_64__) || defined(_M_X64) || defined(__ia64__) || defined(_M_IA64) || defined(__aarch64__) || defined(_M_ARM64)
#define SU_BITS_64
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || defined(_M_ARM)
#define SU_BITS_32
#else
#define SU_BITS_UNKNOWN
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

#ifdef __cplusplus
#define BEGIN_CPSU_CODE() extern "C" {
#define END_CPSU_CODE() }
#else
#define BEGIN_CPSU_CODE()
#define END_CPSU_CODE()
#endif

#endif //SRC_CPSU__CPSU_INCLUDE_H
