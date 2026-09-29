# Shell-With-Features-Proj1

A Unix shell built for COP4610 Project 1: prompt display, environment
variable / tilde expansion, `$PATH` search, external command execution,
I/O redirection, piping, background processing, and built-in commands.

## Status

This repository currently implements the parts assigned to **Ammiel
Bowen** (prompt, `$PATH` search, and I/O redirection as lead; tilde
expansion, external command execution, and internal commands as
support). Parts owned by other teammates are present as clearly marked
`TODO(teammate, Part N)` stubs so the project **compiles and runs**
end-to-end today, but do not yet do their job:

| Part | Owner | Status |
|---|---|---|
| 0. Tokenization | Teammate | Placeholder lexer in `src/tokenizer.c` (whitespace split + splits `< > \| &` into their own tokens). Replace with the real lexer. |
| 1. Prompt | **Ammiel Bowen (lead)** | Done — `src/prompt.c` |
| 2. Environment variables | **Ammiel Bowen** | Done — `src/expand.c` (`expand_env_var`); empty expansions are dropped in `src/main.c` |
| 3. Tilde expansion | **Ammiel Bowen (support)** | Done — `src/expand.c` (`expand_tilde`) |
| 4. `$PATH` search | **Ammiel Bowen (lead)** | Done — `src/path.c` |
| 5. External command execution | **Ammiel Bowen (support)** | Done — `src/executor.c` (`execute_command`) |
| 6. I/O redirection | **Ammiel Bowen (lead)** | Done — `src/executor.c` (`parse_command`, `execute_command`) |
| 7. Piping | Teammate | Stub in `src/executor.c` (`execute_pipeline`); not yet wired into `main.c`. |
| 8. Background processing | **Ammiel Bowen** | Done for single commands (with or without redirection) — `src/jobs.c` (`add_job`, `reap_finished_jobs`), `src/executor.c`. Background pipelines need `execute_pipeline()` (Part 7) to call `add_job()` with the last stage's PID. |
| 9. Internal commands (`exit`, `cd`, `jobs`) | **Ammiel Bowen (support)** | Done — `src/builtins.c`, `src/jobs.c`, `src/history.c` |
| Extra credit | All members | Not attempted yet — depends on Part 7 (piping) for unlimited pipes and pipe+redirection combo. Shell-ception should work "for free" once tokenization/env-var expansion land, since it only needs external command execution (already implemented) — needs verification on Linux. |

Parts 0 and 7 are still stubs: the shell runs commands with `$PATH`
search, tilde/environment-variable expansion, I/O redirection, and
background execution, but piping is not yet functional. Whoever picks up those parts should search this repo for
`TODO(teammate` to find every integration point.

## Project Structure

```
.
├── Makefile              # builds bin/shell
├── README.md
├── include/              # public headers, one per module
│   ├── shell.h           # shared limits (MAX_LINE_LEN, MAX_JOBS, ...)
│   ├── prompt.h          # Part 1
│   ├── tokenizer.h       # Part 0
│   ├── expand.h          # Parts 2 & 3
│   ├── path.h            # Part 4
│   ├── executor.h        # Parts 5, 6, 7 (Command struct, exec, pipeline)
│   ├── jobs.h            # Part 8 data model + Part 9 jobs/exit support
│   ├── builtins.h        # Part 9
│   └── history.h         # Part 9 (exit's "last 3 commands")
├── src/                  # implementations, matching include/ 1:1, plus main.c
│   └── main.c            # read-eval loop tying every module together
└── bin/                  # build output (gitignored); `make` places `shell` here
```

## Building and Running

Requires a POSIX environment with `gcc` and `make` (e.g. `linprog`, or
any Linux/WSL/macOS machine).

```sh
make            # builds bin/shell
./bin/shell      # run it
make clean       # remove build output
```

> Developed and code-reviewed on Windows without a local POSIX
> toolchain available, so it has **not** been compiled locally — please
> build on `linprog` (or WSL/Linux) before relying on it, and file/fix
> any compiler errors found there.

## Division of Labor

- **Ammiel Bowen** — Lead: Prompt (Part 1), `$PATH` Search (Part 4),
  I/O Redirection (Part 6). Support: Tilde Expansion (Part 3), External
  Command Execution (Part 5), Internal Commands (Part 9).
- **[Teammate name]** — Tokenization (Part 0), Environment Variables
  (Part 2), Piping (Part 7), Background Processing (Part 8) — *fill in
  as work is assigned/completed.*

*(Full before/after division-of-labor document to be attached per
Canvas submission requirements.)*

## Development Log

**Ammiel Bowen**
- 2026-09-28: Scaffolded the project (`Makefile`, `include/`, `src/`
  layout). Implemented prompt display (`prompt.c`), `$PATH` search
  (`path.c`), tilde expansion (`expand.c`), external command execution
  and I/O redirection (`executor.c`), and internal commands `exit`,
  `cd`, `jobs` plus their supporting job-table/history data structures
  (`builtins.c`, `jobs.c`, `history.c`). Left clearly-marked stubs for
  tokenization, environment-variable expansion, piping, and background
  processing so a teammate can pick those up without needing to touch
  the rest of the codebase.
- 2026-09-28: Implemented environment-variable expansion (Part 2) and
  background processing (Part 8: job table, `[n] pid` on start,
  `[n]+ done cmd` on completion, non-blocking reaping each prompt).
  Finished wiring external command execution (Part 5) into background
  mode and flushed stdout before `fork()`.

**[Teammate name]**
- *Add entries here as work is completed.*

## Group Meetings

*To be filled in as the team meets.*

## Extra Credit

Not yet implemented. Unlimited pipes and combined piping + I/O
redirection both build on Part 7 (piping), which is still a stub.
Shell-ception (running `bin/shell` from within itself) needs no special
support beyond `$PATH`/execution (already implemented) plus working
tokenization — worth a quick manual test once Part 0 lands.
