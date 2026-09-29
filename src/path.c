#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "path.h"
#include "shell.h"

/* Part 4: $PATH search (Ammiel Bowen, lead). */

static int is_executable_file(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return 0;
    }
    if (!S_ISREG(st.st_mode)) {
        return 0;
    }
    return access(path, X_OK) == 0;
}

int path_search(const char *cmd, char *resolved, size_t resolved_size) {
    if (cmd == NULL || cmd[0] == '\0') {
        return -1;
    }

    /* A slash anywhere in the command means "use this path as-is",
     * matching how Bash treats "./prog" or "/usr/bin/ls". */
    if (strchr(cmd, '/') != NULL) {
        if (is_executable_file(cmd)) {
            strncpy(resolved, cmd, resolved_size - 1);
            resolved[resolved_size - 1] = '\0';
            return 0;
        }
        return -1;
    }

    const char *path_env = getenv("PATH");
    if (path_env == NULL) {
        return -1;
    }

    char *path_copy = strdup(path_env);
    if (path_copy == NULL) {
        return -1;
    }

    int found = -1;
    char *saveptr = NULL;
    char *dir = strtok_r(path_copy, ":", &saveptr);
    while (dir != NULL) {
        char candidate[MAX_PATH_LEN];
        if (dir[0] == '\0') {
            /* An empty PATH entry (leading/trailing/double colon)
             * means "search the current directory", same as Bash. */
            snprintf(candidate, sizeof(candidate), "./%s", cmd);
        } else {
            snprintf(candidate, sizeof(candidate), "%s/%s", dir, cmd);
        }

        if (is_executable_file(candidate)) {
            strncpy(resolved, candidate, resolved_size - 1);
            resolved[resolved_size - 1] = '\0';
            found = 0;
            break;
        }

        dir = strtok_r(NULL, ":", &saveptr);
    }

    free(path_copy);
    return found;
}
