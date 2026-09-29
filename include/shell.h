#ifndef SHELL_H
#define SHELL_H

/* Shared limits used across the whole shell. The project assumptions
 * guarantee commands are under 200 characters and at most 10
 * concurrent background jobs, so these are generous, fixed upper
 * bounds rather than hard protocol limits. */
#define MAX_CMD_LEN   200
#define MAX_LINE_LEN  256
#define MAX_TOKENS    64
#define MAX_JOBS      10
#define MAX_PATH_LEN  4096

#endif /* SHELL_H */
