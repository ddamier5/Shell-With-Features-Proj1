#include <stdio.h>
#include <string.h>

#include "builtins.h"
#include "executor.h"
#include "expand.h"
#include "history.h"
#include "jobs.h"
#include "prompt.h"
#include "shell.h"
#include "tokenizer.h"

int main(void) {
    prompt_init();

    char line[MAX_LINE_LEN];

    while (1) {
        /* Part 8 TODO hook (teammate): reap any background jobs that
         * finished since the last prompt, printing "[n]+ done ...". */
        reap_finished_jobs();

        print_prompt();

        if (fgets(line, sizeof(line), stdin) == NULL) {
            putchar('\n');
            break; /* EOF (Ctrl+D) behaves like exit */
        }

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (line[0] == '\0') {
            continue; /* blank line */
        }

        char raw_copy[MAX_LINE_LEN];
        strncpy(raw_copy, line, sizeof(raw_copy) - 1);
        raw_copy[sizeof(raw_copy) - 1] = '\0';

        char *tokens[MAX_TOKENS];
        int ntokens = tokenize(line, tokens, MAX_TOKENS);
        if (ntokens <= 0) {
            continue;
        }

        /* Parts 2/3: expand every whole-argument token in place. */
        static char expanded_storage[MAX_TOKENS][MAX_LINE_LEN];
        for (int i = 0; i < ntokens; i++) {
            expand_token(tokens[i], expanded_storage[i], sizeof(expanded_storage[i]));
            tokens[i] = expanded_storage[i];
        }

        /*
         * TODO(teammate, Part 7): detect '|' among `tokens` here and,
         * if present, split into per-stage token runs, parse_command()
         * each one, and dispatch to execute_pipeline() instead of the
         * single-command path below.
         */

        Command cmd;
        if (parse_command(tokens, ntokens, &cmd) != 0) {
            fprintf(stderr, "shell: syntax error\n");
            continue;
        }

        if (cmd.argc == 0) {
            continue;
        }

        if (try_builtin(cmd.argv, cmd.argc, raw_copy)) {
            history_add(raw_copy);
            continue;
        }

        int status = execute_command(&cmd);
        if (status != -1) {
            history_add(raw_copy);
        }
    }

    return 0;
}
