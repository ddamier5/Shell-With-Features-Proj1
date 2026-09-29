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

/* Part 6: I/O redirection (Ammiel Bowen, lead) -- token-level parsing. */
int parse_command(char *tokens[], int ntokens, Command *cmd) {
    cmd->argc = 0;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->background = 0;
    cmd->cmdline = NULL;

    for (int i = 0; i < ntokens; i++) {
        if (strcmp(tokens[i], "<") == 0) {
            if (i + 1 >= ntokens || cmd->infile != NULL) {
                return -1;
            }
            cmd->infile = tokens[++i];
        } else if (strcmp(tokens[i], ">") == 0) {
            if (i + 1 >= ntokens || cmd->outfile != NULL) {
                return -1;
            }
            cmd->outfile = tokens[++i];
        } else if (strcmp(tokens[i], "&") == 0) {
            /* Part 8: acted on by execute_command() after the fork. */
            cmd->background = 1;
        } else if (strcmp(tokens[i], "|") == 0) {
            /* Part 7 (piping) is handled by main.c before parse_command()
             * is ever called on a pipeline stage; seeing one here means
             * a single-command parse was attempted on a pipeline. */
            return -1;
        } else {
            if (cmd->argc >= MAX_TOKENS - 1) {
                return -1;
            }
            cmd->argv[cmd->argc++] = tokens[i];
        }
    }

    cmd->argv[cmd->argc] = NULL;
    return 0;
}

/* Part 6: validates the "<" redirection target per spec -- must exist
 * and be a regular file. Checked before forking so a bad target never
 * spawns a doomed child. */
static int validate_infile(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "shell: %s: No such file or directory\n", path);
        return -1;
    }
    if (!S_ISREG(st.st_mode)) {
        fprintf(stderr, "shell: %s: Not a regular file\n", path);
        return -1;
    }
    return 0;
}

/* Part 5 + Part 6 (lead) + Part 8: fork/exec with redirection,
 * optionally in the background. */
int execute_command(Command *cmd) {
    if (cmd->argc == 0) {
        return -1;
    }

    if (cmd->infile != NULL && validate_infile(cmd->infile) != 0) {
        return -1;
    }

    char resolved[MAX_PATH_LEN];
    if (path_search(cmd->argv[0], resolved, sizeof(resolved)) != 0) {
        fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
        return -1;
    }

    /* Flush before forking so buffered shell output isn't duplicated
     * into the child's copy of the stdio buffers. */
    fflush(stdout);

    pid_t pid = fork();
    if (pid < 0) {
        perror("shell: fork");
        return -1;
    }

    if (pid == 0) {
        /* Child: wire up redirection, then exec. */
        if (cmd->infile != NULL) {
            int fd_in = open(cmd->infile, O_RDONLY);
            if (fd_in < 0) {
                fprintf(stderr, "shell: %s: %s\n", cmd->infile, strerror(errno));
                _exit(1);
            }
            if (dup2(fd_in, STDIN_FILENO) < 0) {
                perror("shell: dup2");
                _exit(1);
            }
            close(fd_in);
        }

        if (cmd->outfile != NULL) {
            /* New file, or overwritten in place, always mode 0600. */
            int fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC,
                               S_IRUSR | S_IWUSR);
            if (fd_out < 0) {
                fprintf(stderr, "shell: %s: %s\n", cmd->outfile, strerror(errno));
                _exit(1);
            }
            if (dup2(fd_out, STDOUT_FILENO) < 0) {
                perror("shell: dup2");
                _exit(1);
            }
            close(fd_out);
        }

        execv(resolved, cmd->argv);
        /* execv() only returns on failure. */
        fprintf(stderr, "shell: %s: %s\n", cmd->argv[0], strerror(errno));
        _exit(127);
    }

    /* Part 8: don't wait on a background job; the main loop reaps it
     * later via reap_finished_jobs(). */
    if (cmd->background) {
        add_job(pid, cmd->cmdline != NULL ? cmd->cmdline : cmd->argv[0]);
        return 0;
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("shell: waitpid");
        return -1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

int execute_pipeline(Command *cmds, int n) {
    /*
     * Part 7: Piping — NOT assigned to Ammiel Bowen.
     * TODO(teammate): for i in [0, n), pipe() between stage i and
     * i+1, fork one child per stage, dup2() stdin/stdout to the
     * correct pipe ends (plus cmd->infile/outfile for the first/last
     * stage if extra credit "piping + redirection" is attempted),
     * close all pipe fds in both parent and children, then wait for
     * every child.
     */
    (void)cmds;
    (void)n;
    fprintf(stderr, "shell: piping is not implemented yet\n");
    return -1;
}
