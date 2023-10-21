#pragma once
#ifndef SRC_CPSU_SRC_FSIO_H
#define SRC_CPSU_SRC_FSIO_H

#include "../stuff/_cpsu_include.h"
#include "vector.h"

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
