#ifndef BUILTINS_H
#define BUILTINS_H

/*
 * Part 9: Internal command execution (Ammiel Bowen, support).
 * Built-in commands (exit, cd, jobs) implemented from scratch, with no
 * execv() calls per the project restrictions.
 *
 * If argv[0] names a recognized builtin, handles it (see return values
 * below). Otherwise returns 0 and does nothing, so the caller falls
 * through to external command execution. `raw_line` is the original command line
 * text, used by "exit" if it needs to reference what was typed.
 */
int try_builtin(char *argv[], int argc, const char *raw_line);
/*
 * Return values of try_builtin():
 *    0  argv[0] is not a builtin (caller runs it as an external command)
 *    1  builtin ran successfully (counts as a valid command for history)
 *   -1  builtin was recognized but signaled an error (e.g. bad cd
 *       target); NOT a valid command for history purposes
 */

#endif /* BUILTINS_H */
