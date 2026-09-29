#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>
#include "shell.h"

/* Shared background-job table. Populated by Part 8 (add_job /
 * reap_finished_jobs); read by the Part 9 "jobs" builtin and by exit's
 * wait-for-stragglers behavior. */
typedef struct {
    int job_num;
    pid_t pid;
    char cmdline[MAX_LINE_LEN];
    int active; /* 1 while running, 0 once the slot is free */
} Job;

extern Job job_table[MAX_JOBS];
extern int next_job_num; /* next job number to hand out; never reused */

/*
 * Part 8: Background processing (Ammiel Bowen).
 *  - add_job(): stores pid/cmdline (trailing "&" stripped) in a free
 *    job_table slot under job_num = next_job_num++, prints
 *    "[job_num] pid", and returns job_num (-1 if the table is full).
 *    For a pipeline, pass the PID of the last stage.
 *  - reap_finished_jobs(): call once per main-loop iteration. Reaps
 *    every finished child without blocking, printing
 *    "[job_num]+ done cmdline" and freeing the slot for tracked jobs.
 */
int add_job(pid_t pid, const char *cmdline);
void reap_finished_jobs(void);

/* Part 9 (Ammiel Bowen, support): builtins built on top of the shared
 * job table above. */
void print_jobs(void);    /* "jobs" builtin */
void wait_all_jobs(void); /* used by "exit" to block for stragglers */

#endif /* JOBS_H */
