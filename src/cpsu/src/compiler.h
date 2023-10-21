#pragma once
#ifndef SRC_CPSU_SRC_COMPILER_H
#define SRC_CPSU_SRC_COMPILER_H

#include "../stuff/_cpsu_include.h"
#include "vector.h"

BEGIN_CPSU_CODE()

// C Compiler interface
// a compiler flag represents a single argument
// example:
// gcc -c -o main.o main.c test.c -I include
// cl /c /Fo main.obj main.c test.c /I include
// is represented as:
// suCompilerBoolFlag { SU_COMPILER_FLAG_COMPILE, suTrue }
// suCompilerStringFlag { SU_COMPILER_FLAG_OUTPUT, "main.o" }
// suCompilerStringFlag { SU_COMPILER_FLAG_INPUT, "main.c" }
// suCompilerStringFlag { SU_COMPILER_FLAG_INPUT, "test.c" }
// suCompilerStringFlag { SU_COMPILER_FLAG_INCLUDE, "include" }


/**
 * @brief Represents a single compiler flag
 * @details A compiler flag is a single argument to the compiler
 */
typedef enum {
    SU_COMPILER_FLAG_COMPILE, // bool
    SU_COMPILER_FLAG_ASSEMBLE, // bool
    SU_COMPILER_FLAG_PREPROCESS, // bool
    SU_COMPILER_FLAG_LINK, // bool
    SU_COMPILER_FLAG_OUTPUT, // string
    SU_COMPILER_FLAG_INPUT, // string
    SU_COMPILER_FLAG_INCLUDE, // string
    SU_COMPILER_FLAG_DEFINE, // string
    SU_COMPILER_FLAG_LIBRARY, // string
    SU_COMPILER_FLAG_LIBRARY_PATH, // string
    SU_COMPILER_FLAG_OPTIMIZATION, // int
    SU_COMPILER_FLAG_DEBUG, // bool
    SU_COMPILER_FLAG_WARNING, // int
    SU_COMPILER_FLAG_ERROR // string
} suCompilerFlagType;

const char* suMSVCCompilerFlagTypes[] = {
        "c",
        "FA",
        "",
        "link",
        "o",
        "",
        "I",
        "D",
        "l",
        "L",
        "O",
        "Zi",
        "W",
        "error"
};

const char* suGCCCompilerFlagTypes[] = {
        "c",
        "S",
        "E",
        "",
        "o",
        "",
        "I",
        "D",
        "l",
        "L",
        "O",
        "O",
        "W",
        "error"
};

const char* suClangCompilerFlagTypes[] = {
        "c",
        "S",
        "E",
        "",
        "o",
        "",
        "I",
        "D",
        "l",
        "L",
        "O",
        "O",
        "W",
        "error"
};

const char suCompilerFlagPrefix[] = {
    ' ', // 0 = SU_COMPILER_UNKNOWN
    '-', // 1 = SU_COMPILER_GNUC
    '-', // 2 = SU_COMPILER_LLVM
    '/' // 3 = SU_COMPILER_MSVC
};

/**
 * @brief Represents a boolean compiler flag
 * @details A boolean compiler flag is a flag that is either present or not
 * @see suCompilerFlag
 * @see suCompilerFlagType
 */
typedef struct {
    suCompilerFlagType flag;
    suBool value;
} suCompilerBoolFlag;

/**
 * @brief Represents a string compiler flag
 * @details A string compiler flag is a flag that has a string value
 * @see suCompilerFlag
 * @see suCompilerFlagType
 */
typedef struct {
    suCompilerFlagType flag;
    const char* value;
} suCompilerStringFlag;

/**
 * @brief Represents an integer compiler flag
 * @details An integer compiler flag is a flag that has an integer value
 * @see suCompilerFlag
 * @see suCompilerFlagType
 */
typedef struct {
    suCompilerFlagType flag;
    int32_t value;
} suCompilerIntFlag;

/**
 * @brief Represents a compiler flag's classification
 * @details A compiler flag's classification is boolean, string, or integer
 * @see suCompilerFlag
 * @see suCompilerBoolFlag
 * @see suCompilerStringFlag
 * @see suCompilerIntFlag
 */
typedef enum {
    SU_COMPILER_CLASSIFICATION_BOOL,
    SU_COMPILER_CLASSIFICATION_STRING,
    SU_COMPILER_CLASSIFICATION_INT
} suCompilerFlagClassification;

/**
 * @brief Represents a compiler flag
 * @details A compiler flag is a single argument to the compiler
 * @see suCompilerBoolFlag
 * @see suCompilerStringFlag
 * @see suCompilerIntFlag
 * @see suCompilerFlagClassification
 * @see suCompilerFlagType
 */
typedef struct {
    suCompilerFlagClassification classification;
    union {
        suCompilerBoolFlag boolFlag;
        suCompilerStringFlag stringFlag;
        suCompilerIntFlag intFlag;
    };
} suCompilerFlag;

___suVectorDeclare(suCompilerFlag, CompilerFlag);

/**
 * @brief Represents a compiler backend's type
 * @details A compiler backend is a compiler that is used to compile code
 * @see suCompiler
 */
typedef enum {
    SU_COMPILER_UNKNOWN, // unknown compiler
    SU_COMPILER_GNUC, // gcc, g++, etc.
    SU_COMPILER_LLVM, // cl, link, etc.
    SU_COMPILER_MSVC // clang, clang++, etc.
} suCompilerType;

/**
 * @brief Represents a compiler backend
 * @details A compiler backend is a compiler that is used to compile code
 * @see suCompiler
 *
 */
typedef struct {
    suCompilerType type;
    const char* cCompiler32;
    const char* cxxCompiler32;
    const char* cCompiler64;
    const char* cxxCompiler64;
} suCompilerBackend;

/**
 * @brief Represents a compiler
 * @details A compiler is a program that is used to compile code
 * @see suCompilerBackend
 * @see suCompilerFlag
 * @see suCompilerFlagType
 */
typedef struct {
    suVector_CompilerFlag* flags;
    suCompilerBackend backend;
    suBool cxx;
} suCompiler;

typedef enum {
    SU_COMPILER_PREFERENCE_GNUC_LLVM_MSVC = 0x010203, // 66051
    SU_COMPILER_PREFERENCE_GNUC_LLVM = 0x010200, // 66048
    SU_COMPILER_PREFERENCE_GNUC_MSVC_LLVM = 0x010302, // 66306
    SU_COMPILER_PREFERENCE_GNUC_MSVC = 0x010300, // 66304
    SU_COMPILER_PREFERENCE_GNUC = 0x010000, // 65536
    SU_COMPILER_PREFERENCE_LLVM_GNUC_MSVC = 0x020103, // 131331
    SU_COMPILER_PREFERENCE_LLVM_GNUC = 0x020100, // 131328
    SU_COMPILER_PREFERENCE_LLVM_MSVC_GNUC = 0x020301, // 131841
    SU_COMPILER_PREFERENCE_LLVM_MSVC = 0x020300, // 131840
    SU_COMPILER_PREFERENCE_LLVM = 0x020000, // 131072
    SU_COMPILER_PREFERENCE_MSVC_GNUC_LLVM = 0x030102, // 196866
    SU_COMPILER_PREFERENCE_MSVC_GNUC = 0x030100, // 196864
    SU_COMPILER_PREFERENCE_MSVC_LLVM_GNUC = 0x030201, // 197121
    SU_COMPILER_PREFERENCE_MSVC_LLVM = 0x030200, // 197120
    SU_COMPILER_PREFERENCE_MSVC = 0x030000, // 196608
} suCompilerPreference;

#define SU_FIRST_PREFERENCE_ID(num) ((num >> 16) & 0xFF)
#define SU_SECOND_PREFERENCE_ID(num) ((num >> 8) & 0xFF)
#define SU_THIRD_PREFERENCE_ID(num) (num & 0xFF)
#define SU_FIRST_PREFERENCE(num) (suCompilerPreference) (SU_FIRST_PREFERENCE_ID(num) << 16)
#define SU_SECOND_PREFERENCE(num) (suCompilerPreference) (SU_SECOND_PREFERENCE_ID(num) << 8)
#define SU_THIRD_PREFERENCE(num) (suCompilerPreference) SU_THIRD_PREFERENCE_ID(num)
#define SU_COMPILER_PREFERENCE(first, second, third) (suCompilerPreference) (SU_FIRST_PREFERENCE(first) | SU_SECOND_PREFERENCE(second) | SU_THIRD_PREFERENCE(third))

#define SU_COMPILE_C suFalse
#define SU_COMPILE_CXX suTrue

/**
 * @brief Creates a compiler
 * @details Creates a compiler with the specified flags and preferences
 * @param cxx Whether or not to compile C++ code
 * @param preference The preference of compilers to use
 * @return The created compiler
 * @see suCompiler
 * @see suCompilerFlag
 * @see suCompilerPreference
 */

suCompiler suCompilerNew(suBool cxx, suCompilerPreference preference);

/**
 * @brief Creates a compiler backend
 * @details Attempts to find the compiler executables and creates a compiler backend
 * @param preference The preference of compilers to use
 * @return The created compiler backend
 */
suCompilerBackend suCompilerBackendNew(suCompilerPreference preference);

suCompilerBackend suCompilerBackendGetFromType(suCompilerType type);

suCompilerBackend suCompilerBackendGetGCC();
suCompilerBackend suCompilerBackendGetClang();
suCompilerBackend suCompilerBackendGetMSVC();

/**
 * @brief Adds a boolean flag to the compiler
 * @details Adds a boolean flag to the compiler
 * @param compiler The compiler to add the flag to
 * @param flag The flag to add
 * @see suCompiler
 * @see suCompilerFlag
 * @see suCompilerBoolFlag
 */
void suCompilerAddBoolFlag(suCompiler* compiler, suCompilerBoolFlag flag);

/**
 * @brief Adds a string flag to the compiler
 * @details Adds a string flag to the compiler
 * @param compiler The compiler to add the flag to
 * @param flag The flag to add
 * @see suCompiler
 * @see suCompilerFlag
 * @see suCompilerStringFlag
 */
void suCompilerAddStringFlag(suCompiler* compiler, suCompilerStringFlag flag);

/**
 * @brief Adds an integer flag to the compiler
 * @details Adds an integer flag to the compiler
 * @param compiler The compiler to add the flag to
 * @param flag The flag to add
 * @see suCompiler
 * @see suCompilerFlag
 * @see suCompilerIntFlag
 */
void suCompilerAddIntFlag(suCompiler* compiler, suCompilerIntFlag flag);

/**
 * @brief Adds a flag to the compiler
 * @details Adds a flag to the compiler
 * @param compiler The compiler to add the flag to
 * @param flag The flag to add
 * @see suCompiler
 * @see suCompilerFlag
 */
void suCompilerAddFlag(suCompiler* compiler, suCompilerFlag flag);

/**
 * @brief Sets the output of the compiler
 * @details Sets the output of the compiler
 * @param compiler The compiler to set the output of
 * @param output The output of the compiler
 * @see suCompiler
 * @see suCompilerFlag
 */
void suCompilerSetOutput(suCompiler* compiler, const char* output);

/**
 * @brief Adds an input to the compiler
 * @details Adds an input to the compiler
 * @param compiler The compiler to add the input to
 * @param input The input to add
 * @see suCompiler
 * @see suCompilerFlag
 */
void suCompilerAddInput(suCompiler* compiler, const char* input);

/**
 * @brief Gets the command to run the compiler
 * @param compiler The compiler to get the command of
 * @return The command to run the compiler
 */
const char* suCompilerGetCommand(suCompiler* compiler);

/**
 * @brief Runs the compiler
 * @param compiler The compiler to run
 * @return The exit code of the compiler
 */
uint8_t suCompilerRun(suCompiler* compiler, suBool printCommand);

END_CPSU_CODE()

#endif //SRC_CPSU_SRC_COMPILER_H
