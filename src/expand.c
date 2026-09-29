#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expand.h"

/* Part 3: Tilde expansion (Ammiel Bowen, support). Only a standalone
 * "~" or a leading "~/" is expanded, per the project assumptions. */
void expand_tilde(const char *token, char *out, size_t out_size) {
    if (token[0] != '~' || (token[1] != '\0' && token[1] != '/')) {
        strncpy(out, token, out_size - 1);
        out[out_size - 1] = '\0';
        return;
    }

    const char *home = getenv("HOME");
    if (home == NULL) {
        home = "";
    }

    if (token[1] == '\0') {
        strncpy(out, home, out_size - 1);
        out[out_size - 1] = '\0';
    } else {
        /* token + 1 is "/rest...", so this yields "$HOME/rest..." */
        snprintf(out, out_size, "%s%s", home, token + 1);
    }
}

/* Part 2: Environment variable expansion (Ammiel Bowen). Only whole
 * arguments are expanded, per the project assumptions: "$USER" becomes
 * the value of USER, an unset variable becomes "" (as in Bash), and a
 * lone "$" is left as a literal. */
void expand_env_var(const char *token, char *out, size_t out_size) {
    if (token[0] != '$' || token[1] == '\0') {
        strncpy(out, token, out_size - 1);
        out[out_size - 1] = '\0';
        return;
    }

    const char *value = getenv(token + 1);
    if (value == NULL) {
        value = "";
    }

    strncpy(out, value, out_size - 1);
    out[out_size - 1] = '\0';
}

void expand_token(const char *token, char *out, size_t out_size) {
    if (token[0] == '$') {
        expand_env_var(token, out, out_size);
    } else if (token[0] == '~') {
        expand_tilde(token, out, out_size);
    } else {
        strncpy(out, token, out_size - 1);
        out[out_size - 1] = '\0';
    }
}
