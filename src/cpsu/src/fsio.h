#pragma once
#ifndef SRC_CPSU_SRC_FSIO_H
#define SRC_CPSU_SRC_FSIO_H

#include "../stuff/_cpsu_include.h"
#include "vector.h"

BEGIN_CPSU_CODE()

/**
 * @brief The result of a path query.
 * exists is true if the path exists, false otherwise.
 * isDir is true if the path is a directory, false otherwise.
 * isFile is true if the path is a file, false otherwise.
 * isLink is true if the path is a link, false otherwise.
 * isReadable is true if the path is readable, false otherwise.
 * isWritable is true if the path is writable, false otherwise.
 * isHidden is true if the path is hidden, false otherwise.
 * If success is false, all other fields are undefined.
 * If exists is false, all other fields are undefined.
 * @see suPathQuery
 * @see suPathStringQuery
 * @see suPath
 */
typedef struct {
    suBool exists; // should be checked first
    suBool isDir;
    suBool isFile;
    suBool isLink;
    suBool isReadable;
    suBool isWritable;
    suBool isHidden;
} suPathQueryResult;

/**
 * @brief A path object.
 * This is a wrapper around a string.
 * @see suPathNewDir
 * @see suPathNewFile
 * @see suPathToString
 * @see suPathQuery
 */
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

/**
 * @brief Get the extension of a path.
 * @param path The path to get the extension of.
 * @return The extension of the path. This is a pointer to the extension in the given path, no new memory is allocated.
 */
const char* suPathStringGetExtension(const char *path);

/**
 * @brief Get the extension of a path.
 * @param path The path to get the extension of.
 * @return The extension of the path. This is a pointer to the extension in the given path, no new memory is allocated.
 */
const char* suPathGetExtension(suPath path);


/**
 * @brief Read a file into a vector of bytes.
 * @param path The path to read.
 * @return The vector of bytes.
 */
suVector_u8* suPathStringReadFileBytes(const char *path);

/**
 * @brief Read a file into a vector of bytes.
 * @param path The path to read.
 * @return The vector of bytes.
 */
suVector_u8* suPathReadFileBytes(suPath path);

/**
 * @brief Read a file into a string.
 * @param path The path to read.
 * @return The string.
 */
const char* suPathStringReadFileText(const char *path);

/**
 * @brief Read a file into a string.
 * @param path The path to read.
 * @return The string.
 */
const char* suPathReadFileText(suPath path);

// TODO: File writing

END_CPSU_CODE()

#endif //SRC_CPSU_SRC_FSIO_H
