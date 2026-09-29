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
    const char *cmdline; /* original line, for job messages; set by caller */
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
 * for the child, unless cmd->background is set (Part 8), in which case
 * the child is registered with add_job() under cmd->cmdline and the
 * call returns 0 immediately.
 *
 * Returns the child's exit status (0-255) on a normal run, or -1 if
 * the shell itself couldn't run the command at all (not found, fork
 * failure, bad redirection target) — used by the caller to decide
 * whether the command line counts as "valid" for history purposes.
 */
int execute_command(Command *cmd);

/*
 * Part 7: Piping (Don Damier, lead) -- implemented in src/pipeline.c.
 *
 * Runs `n` Commands connected by pipe(), one forked child per stage,
 * stage i's stdout feeding stage i+1's stdin. Supports any number of
 * stages. cmds[0].infile and cmds[n-1].outfile are honored (extra
 * credit: piping + I/O redirection). If cmds[n-1].background is set the
 * pipeline is not waited on and add_job() gets the LAST stage's PID.
 *
 * Returns the last stage's exit status when run in the foreground, 0
 * after launching a background pipeline, or -1 if the pipeline could
 * not be run (command not found, pipe/fork failure) -- same contract
 * as execute_command().
 */
int execute_pipeline(Command *cmds, int n);

#endif /* EXECUTOR_H */
