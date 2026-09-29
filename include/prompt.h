#ifndef PROMPT_H
#define PROMPT_H

/*
 * Part 1: Prompt (Ammiel Bowen, lead).
 *
 * prompt_init() ensures USER, MACHINE, and PWD are all set as real
 * environment variables at startup (falling back to getpwuid(),
 * gethostname(), and getcwd() respectively when unset), so that both
 * the prompt and plain "echo $USER"-style expansion see consistent
 * values. print_prompt() renders "USER@MACHINE:PWD> " from them.
 */
void prompt_init(void);
void print_prompt(void);

#endif /* PROMPT_H */
