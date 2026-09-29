#ifndef EXPAND_H
#define EXPAND_H

#include <stddef.h>

/*
 * Part 3: Tilde expansion (Ammiel Bowen, support).
 * Expands `token` into `out` (a buffer of out_size bytes), replacing a
 * standalone "~" or a leading "~/" with $HOME. Tokens that don't start
 * with '~' are copied through unchanged.
 */
void expand_tilde(const char *token, char *out, size_t out_size);

/*
 * Part 2: Environment variable expansion (Ammiel Bowen).
 * If `token` is "$NAME", copies getenv("NAME") into `out` (an empty
 * string if unset, matching Bash). A lone "$" and tokens not starting
 * with '$' are copied through unchanged.
 */
void expand_env_var(const char *token, char *out, size_t out_size);

/*
 * Dispatches a single token to expand_env_var() or expand_tilde()
 * based on its first character, or copies it through unchanged.
 * Called once per token by the main loop.
 */
void expand_token(const char *token, char *out, size_t out_size);

#endif /* EXPAND_H */
