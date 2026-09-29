#ifndef HISTORY_H
#define HISTORY_H

/*
 * Part 9: internal command support (Ammiel Bowen, support).
 * Tracks the last three successfully executed ("valid") command
 * lines, for the "exit" builtin to display.
 */
void history_add(const char *cmdline);
void history_print_last_three(void);

#endif /* HISTORY_H */
