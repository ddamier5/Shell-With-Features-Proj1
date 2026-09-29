#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "jobs.h"

Job job_table[MAX_JOBS];
int next_job_num = 1;

int add_job(pid_t pid, const char *cmdline) {
    /*
     * Part 8: Background processing — NOT assigned to Ammiel Bowen.
     * TODO(teammate): find a free (active == 0) slot in job_table,
     * fill in pid/cmdline, set job_num = next_job_num++, active = 1,
     * print "[job_num] pid", and return job_num.
     */
    (void)pid;
    (void)cmdline;
    return -1;
}

void reap_finished_jobs(void) {
    /*
     * Part 8: Background processing — NOT assigned to Ammiel Bowen.
     * TODO(teammate): for each active slot, waitpid(pid, &status,
     * WNOHANG); on a completed process print
     * "[job_num]+ done cmdline" and clear the slot (active = 0).
     */
}

/* Part 9: "jobs" builtin (Ammiel Bowen, support). Reads the shared job
 * table above regardless of who populates it. */
void print_jobs(void) {
    int any = 0;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].active) {
            printf("[%d]+ %d %s\n", job_table[i].job_num,
                   (int)job_table[i].pid, job_table[i].cmdline);
            any = 1;
        }
    }
    if (!any) {
        printf("No active background jobs.\n");
    }
}

/* Part 9: "exit" must wait for stragglers (Ammiel Bowen, support). */
void wait_all_jobs(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].active) {
            int status;
            waitpid(job_table[i].pid, &status, 0);
            job_table[i].active = 0;
        }
    }
}
