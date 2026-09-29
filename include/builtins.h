#ifndef BUILTINS_H
#define BUILTINS_H

/*
 * Part 9: Internal command execution (Ammiel Bowen, support).
 * Built-in commands (exit, cd, jobs) implemented from scratch, with no
 * execv() calls per the project restrictions.
 *
 * If argv[0] names a recognized builtin, handles it and returns 1.
 * Otherwise returns 0 and does nothing, so the caller falls through to
 * external command execution. `raw_line` is the original command line
 * text, used by "exit" if it needs to reference what was typed.
 */
int try_builtin(char *argv[], int argc, const char *raw_line);

#endif /* BUILTINS_H */
