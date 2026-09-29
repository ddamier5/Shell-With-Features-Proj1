#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "builtins.h"
#include "history.h"
#include "jobs.h"
#include "shell.h"

/* Part 9: "cd" builtin (Ammiel Bowen, support). Implemented from
 * scratch with chdir(), no execv(). */
static void builtin_cd(char *argv[], int argc) {
    const char *target;

    if (argc == 1) {
        target = getenv("HOME");
        if (target == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    } else if (argc == 2) {
        target = argv[1];
    } else {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    struct stat st;
    if (stat(target, &st) != 0) {
        fprintf(stderr, "cd: %s: No such file or directory\n", target);
        return;
    }
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "cd: %s: Not a directory\n", target);
        return;
    }
    if (chdir(target) != 0) {
        fprintf(stderr, "cd: %s: %s\n", target, strerror(errno));
        return;
    }

    char cwd[MAX_PATH_LEN];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        setenv("PWD", cwd, 1);
    }
}

int try_builtin(char *argv[], int argc, const char *raw_line) {
    (void)raw_line;

    if (argc == 0) {
        return 0;
    }

    if (strcmp(argv[0], "cd") == 0) {
        builtin_cd(argv, argc);
        return 1;
    }

    if (strcmp(argv[0], "jobs") == 0) {
        print_jobs();
        return 1;
    }

    if (strcmp(argv[0], "exit") == 0) {
        wait_all_jobs();
        history_print_last_three();
        exit(0);
    }

    return 0;
}
