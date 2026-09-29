#include <string.h>

#include "shell.h"
#include "tokenizer.h"

/*
 * Part 0: Tokenization — NOT assigned to Ammiel Bowen. See the note in
 * include/tokenizer.h.
 *
 * This placeholder does a simple whitespace split, additionally
 * splitting the operator characters '<', '>', '|', '&' into their own
 * tokens even when not separated by spaces (e.g. "cmd>out" becomes
 * "cmd", ">", "out"), so redirection/pipe/background detection
 * downstream has something usable to integrate against. It does NOT
 * implement quoting, globs, or other real-lexer features -- replace it
 * with the provided starter code or a proper hand-written lexer.
 */
int tokenize(char *line, char *tokens[], int max_tokens) {
    static char buf[MAX_LINE_LEN * 2];
    int n = 0;
    size_t bi = 0;
    size_t i = 0;
    size_t len = strlen(line);

    if (max_tokens <= 0) {
        return 0;
    }
    tokens[0] = NULL;

    while (i < len && n < max_tokens - 1) {
        while (i < len && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }
        if (i >= len) {
            break;
        }

        if (line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == '&') {
            buf[bi] = line[i];
            buf[bi + 1] = '\0';
            tokens[n++] = &buf[bi];
            bi += 2;
            i++;
            continue;
        }

        size_t start = bi;
        while (i < len && line[i] != ' ' && line[i] != '\t' &&
               line[i] != '<' && line[i] != '>' &&
               line[i] != '|' && line[i] != '&') {
            buf[bi++] = line[i++];
        }
        buf[bi++] = '\0';
        tokens[n++] = &buf[start];
    }

    tokens[n] = NULL;
    return n;
}
