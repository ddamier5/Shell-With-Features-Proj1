#ifndef PATH_H
#define PATH_H

#include <stddef.h>

/*
 * Part 4: $PATH search (Ammiel Bowen, lead).
 *
 * Resolves `cmd` to an executable file path:
 *  - If `cmd` contains a '/', it is used as-is (relative or absolute)
 *    and must itself be an executable regular file.
 *  - Otherwise, each ':'-separated directory in $PATH is searched (an
 *    empty entry, e.g. from a leading/trailing/double colon, means
 *    the current directory) for an executable regular file named
 *    `cmd`.
 *
 * On success, writes the resolved path into `resolved` (a buffer of
 * at least resolved_size bytes) and returns 0. On failure, returns -1
 * and leaves `resolved` untouched.
 */
int path_search(const char *cmd, char *resolved, size_t resolved_size);

#endif /* PATH_H */
