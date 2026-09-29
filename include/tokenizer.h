#ifndef TOKENIZER_H
#define TOKENIZER_H

/*
 * Part 0: Tokenization — NOT assigned to Ammiel Bowen.
 *
 * TODO(teammate): replace this placeholder with the real lexer (start
 * from shell-examples.tar.gz on Canvas, or write your own). It should
 * split a raw input line into tokens, treating "<", ">", "|", and "&"
 * as their own tokens (see the note in src/tokenizer.c about how the
 * current placeholder does this) so redirection/pipe/background
 * detection downstream keeps working.
 *
 * Contract: tokenize() stores up to max_tokens char* into `tokens`
 * (NULL-terminated, i.e. tokens[return value] == NULL) and returns the
 * number of tokens produced. `line` may be modified/used as backing
 * storage. Returns 0 for a blank/whitespace-only line.
 */
int tokenize(char *line, char *tokens[], int max_tokens);

#endif /* TOKENIZER_H */
