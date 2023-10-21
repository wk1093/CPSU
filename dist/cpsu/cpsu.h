#ifndef SRC_CPSU_CPSU_H
#define SRC_CPSU_CPSU_H

/**** start inlining stuff/_cpsu_include.h ****/
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
/**** ended inlining stuff/_cpsu_include.h ****/

/**** start inlining src/vector.h ****/
#ifndef SRC_CPSU_VECTOR_H
#define SRC_CPSU_VECTOR_H

/**** skipping file: ../stuff/_cpsu_include.h ****/

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
/**** ended inlining src/vector.h ****/
/**** start inlining src/fsio.h ****/
#ifndef SRC_CPSU_SRC_FSIO_H
#define SRC_CPSU_SRC_FSIO_H

/**** skipping file: ../stuff/_cpsu_include.h ****/
/**** skipping file: vector.h ****/

BEGIN_CPSU_CODE()

typedef struct {
    suBool success; // should be checked before using other fields
    suBool exists;
    suBool isDir;
    suBool isFile;
    suBool isLink;
    suBool isReadable;
    suBool isWritable;
    suBool isHidden;
} suPathQueryResult;

typedef struct {
    suVector_str *dirs; // vector of strings
    char *filename;
} suPath;


/**
 * @brief Create a new path object from a string.
 * This path represents a directory.
 * @param path The path string.
 * @return The path object.
 */
suPath suPathNewDir(const char *path);

/**
 * @brief Create a new path object from a string.
 * This path represents a file.
 * @param path The path string.
 * @return The path object.
 */
suPath suPathNewFile(const char *path); // forces the last part of the path to be a file

/**
 * @brief Create a new path object from a string.
 * @param p The path object.
 * @return The path string.
 */
const char* suPathToString(suPath p);

/**
 * @brief Query the status of a given path.
 * @param path The path to query.
 * @return The query result.
 */
suPathQueryResult suPathStringQuery(const char *path);

/**
 * @brief Query the status of a given path.
 * @param path The path to query.
 * @return The query result.
 */
 suPathQueryResult suPathQuery(suPath path);

/**
 * @brief Get the basename of a path.
 * @param path The path to get the basename of.
 * @return The basename of the path. This is a pointer to the basename in the given path, no new memory is allocated.
 * If the path is a directory, the basename is nothing.
 */
const char* suPathStringGetBasename(const char *path);

/**
 * @brief Get the basename of a path.
 * @param path The path to get the basename of.
 * @return The basename of the path. This is a pointer to the basename in the given path, no new memory is allocated.
 * If the path is a directory, the basename is nothing.
 */
 const char* suPathGetBasename(suPath path);


END_CPSU_CODE()

#endif //SRC_CPSU_SRC_FSIO_H
/**** ended inlining src/fsio.h ****/
/**** start inlining src/compiler.h ****/
#ifndef SRC_CPSU_SRC_COMPILER_H
#define SRC_CPSU_SRC_COMPILER_H

/**** skipping file: ../stuff/_cpsu_include.h ****/
/**** skipping file: vector.h ****/

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
/**** ended inlining src/compiler.h ****/

#ifdef CPSU_IMPLEMENTATION

/**** start inlining src/vector.c ****/
/**** skipping file: vector.h ****/

BEGIN_CPSU_CODE()

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

END_CPSU_CODE()
/**** ended inlining src/vector.c ****/
/**** start inlining src/fsio.c ****/
/**** skipping file: fsio.h ****/

BEGIN_CPSU_CODE()

suPath suPathNewDir(const char *path) {
    suPath p;
    p.dirs = suVectorNew_str();
    p.filename = NULL;

    char* pathCopy = malloc(strlen(path) + 1);
    strcpy(pathCopy, path);

    size_t i = 0;
    while (pathCopy[i] != '\0') {
        if (pathCopy[i] == '/' || pathCopy[i] == '\\') {
            pathCopy[i] = '\0';
            suVectorAdd_str(p.dirs, pathCopy);
            pathCopy = pathCopy + i + 1;
            i = 0;
        } else {
            i++;
        }
    }
    if (pathCopy[0] == '\0') {
        return p;
    }
    // add the last part of the path
    suVectorAdd_str(p.dirs, pathCopy);

    return p;
}

suPath suPathNewFile(const char *path) {
    // same as above, but last part isn't included in dirs, its in filename
    suPath p;
    p.dirs = suVectorNew_str();
    p.filename = NULL;

    char* pathCopy = malloc(strlen(path) + 1);
    strcpy(pathCopy, path);

    size_t i = 0;

    while (pathCopy[i] != '\0') {
        if (pathCopy[i] == '/' || pathCopy[i] == '\\') {
            pathCopy[i] = '\0';
            suVectorAdd_str(p.dirs, pathCopy);
            pathCopy = pathCopy + i + 1;
            i = 0;
        } else {
            i++;
        }
    }

    p.filename = pathCopy;

    return p;
}

const char* suPathToString(suPath p) {
    size_t len = 0;
    for (size_t i = 0; i < p.dirs->size; i++) {
        len += strlen(suVectorGet_str(p.dirs, i)) + 1;
    }
    if (p.filename != NULL) {
        len += strlen(p.filename) + 1;
    }

    char* path = malloc(len);
    path[0] = '\0';

    for (size_t i = 0; i < p.dirs->size; i++) {
        strcat(path, suVectorGet_str(p.dirs, i));
        strcat(path, "/");
    }
    if (p.filename != NULL) {
        strcat(path, p.filename);
    }

    return path;
}

suPathQueryResult suPathStringQuery(const char *path) {
    suPathQueryResult sfs = {suFalse};
#ifdef SU_PLATFORM_WINDOWS
    DWORD attr = GetFileAttributesA(path);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return sfs;
    }
    sfs.success = suTrue;
    sfs.exists = suTrue;
    sfs.isDir = attr & FILE_ATTRIBUTE_DIRECTORY;
    sfs.isFile = attr & FILE_ATTRIBUTE_NORMAL;
    sfs.isLink = attr & FILE_ATTRIBUTE_REPARSE_POINT;
    sfs.isReadable = suTrue;
    sfs.isWritable = !(attr & FILE_ATTRIBUTE_READONLY);
    sfs.isHidden = attr & FILE_ATTRIBUTE_HIDDEN;
#else
    struct stat st;
    if (stat(path, &st) != 0) {
        return sfs;
    }
    sfs.success = suTrue;
    sfs.exists = suTrue;
    sfs.isDir = S_ISDIR(st.st_mode);
    sfs.isFile = S_ISREG(st.st_mode);
    sfs.isLink = S_ISLNK(st.st_mode);
    sfs.isReadable = (st.st_mode & S_IRUSR) || (st.st_mode & S_IRGRP) || (st.st_mode & S_IROTH);
    sfs.isWritable = (st.st_mode & S_IWUSR) || (st.st_mode & S_IWGRP) || (st.st_mode & S_IWOTH);
    sfs.isHidden = suPathGetBasename(path)[0] == '.';
#endif
    return sfs;
}

suPathQueryResult suPathQuery(suPath path) {
    const char* pathStr = suPathToString(path);
    suPathQueryResult sfs = suPathStringQuery(pathStr);
    return sfs;
}

const char* suPathStringGetBasename(const char *path) {
    if (path == NULL) {
        return NULL;
    }
    size_t len = strlen(path);
    // this check skips some unnecessary queries (which aren't super expensive, but still)
    if (path[len - 1] == '/' || path[len - 1] == '\\') {
        return NULL;
    }
    suPathQueryResult sfs = suPathStringQuery(path);
    if (!sfs.success || !sfs.exists || sfs.isDir) {
        return NULL;
    }
    suPath p = suPathNewFile(path);
    return p.filename;
}

const char* suPathGetBasename(suPath path) {
    if (path.filename != NULL) {
        return path.filename;
    }
    return NULL;
}




END_CPSU_CODE()
/**** ended inlining src/fsio.c ****/
/**** start inlining src/compiler.c ****/
/**** skipping file: compiler.h ****/

BEGIN_CPSU_CODE()

___suVectorDefine(suCompilerFlag, CompilerFlag);

suCompiler suCompilerNew(suBool cxx, suCompilerPreference preference) {
    suCompilerBackend backend = suCompilerBackendNew(preference);
    suCompiler compiler = { .cxx = cxx, .flags = suVectorNew_CompilerFlag(), .backend=backend };
    return compiler;
}

suCompilerBackend suCompilerBackendNew(suCompilerPreference preference) {
    suCompilerBackend be;
    be = suCompilerBackendGetFromType(SU_FIRST_PREFERENCE_ID(preference));
    if (be.type != SU_COMPILER_UNKNOWN) {
        return be;
    }
    be = suCompilerBackendGetFromType(SU_SECOND_PREFERENCE_ID(preference));
    if (be.type != SU_COMPILER_UNKNOWN) {
        return be;
    }
    be = suCompilerBackendGetFromType(SU_THIRD_PREFERENCE_ID(preference));
    if (be.type != SU_COMPILER_UNKNOWN) {
        return be;
    }
    fprintf(stderr, "No compiler backend found for preference %d\n", preference);
    exit(1);
}

suCompilerBackend suCompilerBackendGetFromType(suCompilerType type) {
    suCompilerBackend be;
    switch (type) {
        case SU_COMPILER_GNUC:
            be = suCompilerBackendGetGCC();
            break;
        case SU_COMPILER_LLVM:
            be = suCompilerBackendGetClang();
            break;
        case SU_COMPILER_MSVC:
            be = suCompilerBackendGetMSVC();
            break;
        default:
            be.type = SU_COMPILER_UNKNOWN;
            break;
    }
    return be;
}

suCompilerBackend suCompilerBackendGetGCC() {
    suCompilerBackend be;
    be.type = SU_COMPILER_UNKNOWN; // if error return be; if success set be.type

    // gcc should be in PATH

    int32_t ret = system("gcc --version > /dev/null 2>&1");
    int32_t ret2 = system("g++ --version > /dev/null 2>&1");
    if (ret == 0) {
        be.type = SU_COMPILER_GNUC;
        be.cCompiler32 = "gcc";
        be.cCompiler64 = "gcc";
    }
    if (ret2 == 0) {
        be.type = SU_COMPILER_GNUC;
        be.cxxCompiler32 = "g++";
        be.cxxCompiler64 = "g++";
    }
    return be;
}

suCompilerBackend suCompilerBackendGetClang() {
    suCompilerBackend be;
    be.type = SU_COMPILER_UNKNOWN; // if error return be; if success set be.type

    // clang should be in PATH
    // if it isn't, it might be windows, and clang can be in VSINSTALLDIR/VC/Tools/Llvm/bin/clang.exe
    // for now just look in PATH

    int32_t ret = system("clang --version > /dev/null 2>&1");
    int32_t ret2 = system("clang++ --version > /dev/null 2>&1");
    if (ret == 0) {
        be.type = SU_COMPILER_LLVM;
        be.cCompiler32 = "clang";
        be.cCompiler64 = "clang";
    }
    if (ret2 == 0) {
        be.type = SU_COMPILER_LLVM;
        be.cxxCompiler32 = "clang++";
        be.cxxCompiler64 = "clang++";
    }
    return be;
}

suCompilerBackend suCompilerBackendGetMSVC() {
    suCompilerBackend be;
    be.type = SU_COMPILER_UNKNOWN; // if error return be; if success set be.type

    // if msvc was in path, it would be so much easier
    // but by default it is not, so we have to find it
    // use VSINSTALLDIR env var to help find it
    // for now (maybe change later) if VSINSTALLDIR is not set, fail

    char* vsinstalldir = getenv("VSINSTALLDIR");
    if (vsinstalldir == NULL || vsinstalldir[0] == '\0') {
        fprintf(stderr, "VSINSTALLDIR not set\n");
        return be;
    }

    // VSINSTALLDIR is set, so we can try to find cl.exe
    // check areas:
    // VSINSTALLDIR/VC/Tools/MSVC/{SOME_VERSION}/bin/Host{OUR_ARCH}/{ARCH_DEP}/cl.exe

//    const char* dir_to_version = "/VC/Tools/MSVC/";
//#ifdef _WIN64
//    const char* dir_to_bin = "/bin/Hostx64/";
//#else
//    const char* dir_to_bin = "/bin/Hostx32/";
//#endif

    // find version dir name
    // find first directory in VSINSTALLDIR/VC/Tools/MSVC/
    // TODO: suFileUtils
    fprintf(stderr, "MSVC not supported yet\n");
    return be;
}

void suCompilerSetOutput(suCompiler* compiler, const char* output) {
    suCompilerStringFlag sf = { .flag = SU_COMPILER_FLAG_OUTPUT, .value = output };
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_STRING, .stringFlag = sf };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddInput(suCompiler* compiler, const char* input) {
    suCompilerStringFlag sf = { .flag = SU_COMPILER_FLAG_INPUT, .value = input };
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_STRING, .stringFlag = sf };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddFlag(suCompiler* compiler, suCompilerFlag flag) {
    suVectorAdd_CompilerFlag(compiler->flags, flag);
}

void suCompilerAddStringFlag(suCompiler* compiler, suCompilerStringFlag flag) {
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_STRING, .stringFlag = flag };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddIntFlag(suCompiler* compiler, suCompilerIntFlag flag) {
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_INT, .intFlag = flag };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddBoolFlag(suCompiler* compiler, suCompilerBoolFlag flag) {
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_BOOL, .boolFlag = flag };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

const char* suCompilerGetCommand(suCompiler* compiler) {
    char command[1024] = { 0 };
    char* ptr = command;

#ifdef SU_BITS_64
    // add 64 bit compiler cxx or c
    if (compiler->cxx) {
        ptr += sprintf(ptr, "%s ", compiler->backend.cxxCompiler64);
    } else {
        ptr += sprintf(ptr, "%s ", compiler->backend.cCompiler64);
    }
#else
    // add 32 bit compiler cxx or c
    if (compiler->cxx) {
        ptr += sprintf(ptr, "%s ", compiler->backend.cxxCompiler32);
    } else {
        ptr += sprintf(ptr, "%s ", compiler->backend.cCompiler32);
    }
#endif

    const char** flags = NULL;
    switch (compiler->backend.type) {
        case SU_COMPILER_GNUC:
            flags = suGCCCompilerFlagTypes;
            break;
        case SU_COMPILER_LLVM:
            flags = suClangCompilerFlagTypes;
            break;
        case SU_COMPILER_MSVC:
            flags = suMSVCCompilerFlagTypes;
            break;
        default:
            fprintf(stderr, "Unknown compiler type %d\n", compiler->backend.type);
            exit(1);
    }

    char prefix = suCompilerFlagPrefix[compiler->backend.type];

    // add flags
    for (uint32_t i = 0; i < compiler->flags->size; i++) {
        suCompilerFlag flag = compiler->flags->data[i];
        switch (flag.classification) {
            case SU_COMPILER_CLASSIFICATION_BOOL:
                if (flags[flag.boolFlag.flag] == NULL || flags[flag.boolFlag.flag][0] == '\0') {
                    fprintf(stderr, "Unknown flag %d\n", flag.boolFlag.flag);
                }
                ptr += sprintf(ptr, "%c%s ", prefix, flags[flag.boolFlag.flag]);
                break;
            case SU_COMPILER_CLASSIFICATION_STRING:
                if (flags[flag.stringFlag.flag] == NULL || flags[flag.stringFlag.flag][0] == '\0') {
                    ptr += sprintf(ptr, " %s ", flag.stringFlag.value);
                } else {
                    ptr += sprintf(ptr, "%c%s %s ", prefix, flags[flag.stringFlag.flag], flag.stringFlag.value);
                }
                break;
            case SU_COMPILER_CLASSIFICATION_INT:
                if (flags[flag.intFlag.flag] == NULL || flags[flag.intFlag.flag][0] == '\0') {
                    ptr += sprintf(ptr, " %d ", flag.intFlag.value);
                } else {
                    ptr += sprintf(ptr, "%c%s %d ", prefix, flags[flag.intFlag.flag], flag.intFlag.value);
                }
                break;
        }
    }

    return strdup(command);
}

uint8_t suCompilerRun(suCompiler* compiler, suBool printCommand) {
    const char* command = suCompilerGetCommand(compiler);
    if (printCommand) {
        printf("%s\n", command);
    }
    uint8_t ret = system(command);
    free((void*)command);
    return ret;
}


END_CPSU_CODE()
/**** ended inlining src/compiler.c ****/

#endif //CPSU_IMPLEMENTATION

/**** start inlining stuff/_cpsu_end.h ****/
#ifndef SRC_CPSU__CPSU_END_H
#define SRC_CPSU__CPSU_END_H

#undef int
#undef long
#undef short
#undef char
#undef signed
#undef unsigned


#endif //SRC_CPSU__CPSU_END_H
/**** ended inlining stuff/_cpsu_end.h ****/

#endif //SRC_CPSU_CPSU_H
