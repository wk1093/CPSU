#include "fsio.h"

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
    len += strlen(p.filename) + 1;

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
