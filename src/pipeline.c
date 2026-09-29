/*
 * Part 7: Piping (Don Damier, lead).
 *
 * Implements execute_pipeline() declared in executor.h.
 *
 * Handles any number of stages (extra credit: unlimited pipes). If the
 * first command has an infile and/or the last has an outfile, those are
 * applied too (extra credit: piping + I/O redirection).
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "executor.h"
#include "jobs.h"
#include "path.h"
#include "shell.h"

/* Close both ends of every pipe in the array. */
static void close_all_pipes(int (*pipes)[2], int npipes) {
    for (int i = 0; i < npipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

/* Child-side: point stdin at `path` (must be a regular file). */
static void redirect_stdin_from(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0 || !S_ISREG(st.st_mode)) {
        fprintf(stderr, "%s: no such regular file\n", path);
        _exit(1);
    }
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror(path);
        _exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

/* Child-side: point stdout at `path`, created/truncated with -rw-------. */
static void redirect_stdout_to(const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror(path);
        _exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

/* Join the stages' argv as "cmd1 args | cmd2 args" for the job table. */
static void build_cmdline(Command *cmds, int n, char *out, size_t size) {
    out[0] = '\0';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < cmds[i].argc; j++) {
            if (j > 0 || i > 0) {
                strncat(out, (j == 0) ? " | " : " ", size - strlen(out) - 1);
            }
            strncat(out, cmds[i].argv[j], size - strlen(out) - 1);
        }
    }
}

int execute_pipeline(Command *cmds, int n) {
    if (n < 1) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (cmds[i].argc < MAX_TOKENS) {
            cmds[i].argv[cmds[i].argc] = NULL;  /* execv needs NULL-terminated */
        }
    }

    /* Resolve every stage first so a typo runs nothing at all. */
    char (*resolved)[MAX_PATH_LEN] = malloc((size_t)n * MAX_PATH_LEN);
    if (resolved == NULL) {
        return -1;
    }
    for (int i = 0; i < n; i++) {
        if (cmds[i].argc == 0 || path_search(cmds[i].argv[0], resolved[i],
                                             MAX_PATH_LEN) != 0) {
            fprintf(stderr, "%s: command not found\n",
                    cmds[i].argc ? cmds[i].argv[0] : "(empty)");
            free(resolved);
            return -1;
        }
    }

    int npipes = n - 1;
    int (*pipes)[2] = malloc((size_t)(npipes > 0 ? npipes : 1) * sizeof(int[2]));
    pid_t *pids = calloc((size_t)n, sizeof(pid_t));
    if (pipes == NULL || pids == NULL) {
        free(resolved);
        free(pipes);
        free(pids);
        return -1;
    }

    for (int i = 0; i < npipes; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            close_all_pipes(pipes, i);
            free(resolved);
            free(pipes);
            free(pids);
            return -1;
        }
    }

    fflush(stdout);  /* children must not inherit buffered output */

    int launched = 0;
    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            break;
        }
        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < n - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            close_all_pipes(pipes, npipes);

            if (i == 0 && cmds[i].infile != NULL) {
                redirect_stdin_from(cmds[i].infile);
            }
            if (i == n - 1 && cmds[i].outfile != NULL) {
                redirect_stdout_to(cmds[i].outfile);
            }

            execv(resolved[i], cmds[i].argv);
            perror(cmds[i].argv[0]);
            _exit(127);
        }
        launched++;
    }

    /* Parent must close its copies or readers never see EOF. */
    close_all_pipes(pipes, npipes);
    free(pipes);

    int result = 0;
    if (launched < n) {
        /* fork failed part-way: reap what started and report failure. */
        for (int i = 0; i < launched; i++) {
            waitpid(pids[i], NULL, 0);
        }
        result = -1;
    } else if (cmds[n - 1].background) {
        /* Report the LAST stage's PID; don't wait. */
        char cmdline[MAX_LINE_LEN];
        build_cmdline(cmds, n, cmdline, sizeof(cmdline));
        add_job(pids[n - 1], cmdline);
    } else {
        for (int i = 0; i < n; i++) {
            int status;
            while (waitpid(pids[i], &status, 0) < 0 && errno == EINTR) {
            }
            if (i == n - 1) {
                if (WIFEXITED(status)) {
                    result = WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)) {
                    result = 128 + WTERMSIG(status);
                }
            }
        }
    }

    free(resolved);
    free(pids);
    return result;
}
