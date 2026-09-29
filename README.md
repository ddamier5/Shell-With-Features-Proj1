# Shell-With-Features-Proj1

A Unix shell written in C for COP4610 Project 1. It shows a
`USER@MACHINE:PWD>` prompt and supports environment-variable and tilde
expansion, `$PATH` search, external command execution, I/O redirection,
piping, background processing, and the built-ins `exit`, `cd` and `jobs`.

## Group Members
- **Ammiel Bowen**: [FSU email]
- **Don Damier**: dd23t@fsu.edu
- **Widens Filsaime**: [FSU email]

## Division of Labor

### Part 0: Tokenization
- **Responsibilities**: Split an input line into tokens; `<`, `>`, `|` and
  `&` become their own tokens (placeholder lexer in `src/tokenizer.c`).
- **Assigned to**: Group

### Part 1: Prompt
- **Responsibilities**: Print `USER@MACHINE:PWD>` from the environment.
- **Assigned to**: Ammiel Bowen (lead), Widens Filsaime (support)

### Part 2: Environment Variables
- **Responsibilities**: Replace whole-argument `$VAR` tokens with their
  values (unset variables expand to nothing).
- **Assigned to**: Widens Filsaime (lead), Don Damier (support)

### Part 3: Tilde Expansion
- **Responsibilities**: Expand a standalone `~` or a leading `~/` to `$HOME`.
- **Assigned to**: Don Damier (lead), Ammiel Bowen (support)

### Part 4: $PATH Search
- **Responsibilities**: Find the executable for commands without a `/` by
  searching each directory in `$PATH`; report "command not found".
- **Assigned to**: Ammiel Bowen (lead), Don Damier (support)

### Part 5: External Command Execution
- **Responsibilities**: `fork()` and `execv()` external commands with arguments.
- **Assigned to**: Widens Filsaime (lead), Ammiel Bowen (support)

### Part 6: I/O Redirection
- **Responsibilities**: `<` and `>` (output files created with mode 0600,
  overwritten if they exist).
- **Assigned to**: Ammiel Bowen (lead), Widens Filsaime (support)

### Part 7: Piping
- **Responsibilities**: Run commands connected by pipes, one forked child per
  stage (`src/pipeline.c`).
- **Assigned to**: Don Damier (lead), Widens Filsaime (support)

### Part 8: Background Processing
- **Responsibilities**: Run commands with `&` without waiting; print
  `[job] pid` on start and `[job]+ done cmd` on completion.
- **Assigned to**: Widens Filsaime (lead), Don Damier (support)

### Part 9: Internal Command Execution
- **Responsibilities**: `exit` (waits for jobs, prints last valid commands),
  `cd` (with error checks) and `jobs`.
- **Assigned to**: Don Damier (lead), Ammiel Bowen (support)

### Extra Credit
- **Responsibilities**: Unlimited pipes; piping combined with I/O redirection.
- **Assigned to**: All members

## File Listing
```
root/
├── src/
│   ├── main.c        # read-eval loop: read, tokenize, expand, dispatch
│   ├── tokenizer.c   # splits a line into tokens (placeholder lexer)
│   ├── prompt.c      # prompt setup and printing
│   ├── expand.c      # environment-variable and tilde expansion
│   ├── path.c        # $PATH search
│   ├── executor.c    # command parsing, redirection, fork/execv
│   ├── pipeline.c    # pipelines (any number of stages)
│   ├── jobs.c        # background job table, reaping, `jobs`
│   ├── builtins.c    # exit, cd, jobs dispatch
│   └── history.c     # last valid commands for `exit`
├── include/          # one header per module, plus shell.h (shared limits)
├── bin/              # build output: bin/shell (created by make, not committed)
├── obj/              # object files (created by make, not committed)
├── Makefile
└── README.md
```

## How to Compile & Execute

### Requirements
- **Compiler**: `gcc` (C11) and `make` on a POSIX system such as linprog.
- **Dependencies**: none.

### Compilation
```bash
make
```
This builds the executable `bin/shell` (object files go in `obj/`).
`make clean` removes `obj/` and `bin/`.

### Execution
```bash
./bin/shell
```
This starts the shell. Type `exit` to leave it.

## Development Log
Each member records their contributions here.

### Ammiel Bowen

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-28 | Scaffolded the project (`Makefile`, `include/`, `src/` layout). Implemented prompt display (`prompt.c`), `$PATH` search (`path.c`), tilde expansion (`expand.c`), external command execution and I/O redirection (`executor.c`), and internal commands `exit`, `cd`, `jobs` plus their supporting job-table/history data structures (`builtins.c`, `jobs.c`, `history.c`). Left clearly-marked stubs for tokenization, environment-variable expansion, piping, and background processing so a teammate can pick those up without needing to touch the rest of the codebase. |
| 2026-09-28 | Implemented environment-variable expansion (Part 2) and background processing (Part 8: job table, `[n] pid` on start, `[n]+ done cmd` on completion, non-blocking reaping each prompt). Finished wiring external command execution (Part 5) into background mode and flushed stdout before `fork()`. |

### Don Damier

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-09-28 | Implemented piping in `pipeline.c` (one child per stage, any number of pipes, `<` on the first stage and `>` on the last) and wired `\|` handling into `main.c`, including background pipelines. |
| 2026-09-28 | Made `try_builtin()` report errors so a failed `cd` is not a valid command; changed the `exit` summary to match the sample run. |

### Widens Filsaime

| Date       | Work Completed / Notes |
|------------|------------------------|
| YYYY-MM-DD | [Widens: add your entries here] |

## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| YYYY-MM-DD | [Names]              | [Agenda items]   | [Actions/Next steps]  |

## Bugs
- **Placeholder lexer**: `src/tokenizer.c` only splits on spaces/tabs and
  `< > | &`; quotes, globs and escapes are not handled (not required).
- **Command length**: lines are limited to `MAX_LINE_LEN` (256) characters and
  `MAX_TOKENS` (64) tokens.

## Extra Credit
- **Unlimited pipes**: `execute_pipeline()` in `src/pipeline.c` creates one pipe
  per adjacent pair of commands, so the number of stages is not fixed.
- **Piping + I/O redirection**: the first stage honors `<` and the last stage
  honors `>`, e.g. `cat < in.txt | sort | head -n 2 > out.txt`.
- **Shell-ception**: `./bin/shell` can be run from inside the shell; it needs no
  special support beyond `$PATH` search and external execution.

## Considerations
- Background pipelines report the PID of the last stage, as the spec requires.
- `exit` prints the last three valid commands (or only the last one if fewer
  than three were valid). Commands that fail (not found, bad `cd`, bad
  redirection) are not counted as valid.
