#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>
#include "shell.h"

/* Shared background-job table. Populating it is Part 8 (not assigned
 * to Ammiel Bowen); reading it for the "jobs" builtin and for exit's
 * wait-for-stragglers behavior is Part 9 (assigned, see jobs.c). */
typedef struct {
    int job_num;
    pid_t pid;
    char cmdline[MAX_LINE_LEN];
    int active; /* 1 while running, 0 once the slot is free */
} Job;

extern Job job_table[MAX_JOBS];
extern int next_job_num; /* next job number to hand out; never reused */

/*
 * Part 8: Background processing — NOT assigned to Ammiel Bowen.
 * TODO(teammate):
 *  - add_job(): find a free job_table slot, fill in pid/cmdline,
 *    assign job_num = next_job_num++, print "[job_num] pid", and
 *    return job_num (currently a no-op that returns -1).
 *  - reap_finished_jobs(): call once per main-loop iteration; use a
 *    non-blocking waitpid(..., WNOHANG) over active slots, and on
 *    completion print "[job_num]+ done cmdline" and free the slot
 *    (currently a no-op).
 */
int add_job(pid_t pid, const char *cmdline);
void reap_finished_jobs(void);

/* Part 9 (Ammiel Bowen, support): builtins built on top of the shared
 * job table above. */
void print_jobs(void);    /* "jobs" builtin */
void wait_all_jobs(void); /* used by "exit" to block for stragglers */

#endif /* JOBS_H */
