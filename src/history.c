#include <stdio.h>
#include <string.h>

#include "history.h"
#include "shell.h"

/* Part 9: internal command support (Ammiel Bowen, support). Tracks the
 * last three valid command lines for "exit" to display. */

#define HISTORY_SIZE 3

static char history[HISTORY_SIZE][MAX_LINE_LEN];
static int history_count = 0; /* how many of the slots below are filled, capped at HISTORY_SIZE */
static int history_total = 0; /* how many valid commands have ever been seen */

void history_add(const char *cmdline) {
    for (int i = 0; i < HISTORY_SIZE - 1; i++) {
        strncpy(history[i], history[i + 1], MAX_LINE_LEN - 1);
        history[i][MAX_LINE_LEN - 1] = '\0';
    }
    strncpy(history[HISTORY_SIZE - 1], cmdline, MAX_LINE_LEN - 1);
    history[HISTORY_SIZE - 1][MAX_LINE_LEN - 1] = '\0';

    if (history_count < HISTORY_SIZE) {
        history_count++;
    }
    history_total++;
}

void history_print_last_three(void) {
    if (history_total == 0) {
        printf("No valid commands were entered.\n");
        return;
    }

    printf("Last %d valid command%s:\n", history_count, history_count == 1 ? "" : "s");
    for (int i = HISTORY_SIZE - history_count; i < HISTORY_SIZE; i++) {
        printf("  %s\n", history[i]);
    }
}
