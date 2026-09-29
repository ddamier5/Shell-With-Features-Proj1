#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "shell.h"

/* A single parsed command: argv ready for execv(), plus any
 * redirection/background info pulled out of the token stream. */
typedef struct {
    char *argv[MAX_TOKENS];
    int argc;
    char *infile;    /* NULL if no "<" redirection was given */
    char *outfile;   /* NULL if no ">" redirection was given */
    int background;  /* 1 if a trailing "&" was seen */
} Command;

/*
 * Part 6: I/O redirection (Ammiel Bowen, lead), plus general command
 * parsing needed to get there.
 *
 * Scans already-tokenized, already-expanded tokens for "<", ">", and
 * "&", pulling the filenames (and the background flag) out of `cmd`
 * and leaving the remaining tokens in cmd->argv. Works regardless of
 * whether "<"/">" appear before or after each other. Returns 0 on
 * success, -1 on a syntax error (e.g. "<" with no filename after it).
 */
int parse_command(char *tokens[], int ntokens, Command *cmd);

/*
 * Part 5 (Ammiel Bowen, support) + Part 6 (Ammiel Bowen, lead).
 *
 * Resolves cmd->argv[0] via path_search(), forks, and in the child
 * wires up cmd->infile/cmd->outfile (validating the input file exists
 * and is a regular file, and creating/truncating the output file with
 * mode 0600) before execv()-ing the resolved path. The parent waits
 * for the child.
 *
 * NOTE(teammate, Part 8): cmd->background is recorded but not yet
 * acted on here — see the comment in executor.c. Wire up add_job()
 * and skip the wait when it's set.
 *
 * Returns the child's exit status (0-255) on a normal run, or -1 if
 * the shell itself couldn't run the command at all (not found, fork
 * failure, bad redirection target) — used by the caller to decide
 * whether the command line counts as "valid" for history purposes.
 */
int execute_command(Command *cmd);

/*
 * Part 7: Piping — NOT assigned to Ammiel Bowen.
 * TODO(teammate): chain `n` Commands with pipe()+dup2(), forking one
 * child per stage and connecting stage i's stdout to stage i+1's
 * stdin. Not yet called from main.c — wire up '|' detection there.
 */
int execute_pipeline(Command *cmds, int n);

#endif /* EXECUTOR_H */
