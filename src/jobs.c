#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "jobs.h"

Job job_table[MAX_JOBS];
int next_job_num = 1;

/* Part 8: Background processing (Ammiel Bowen). */

/* Copies `cmdline` into `out`, dropping the trailing "&" (and any
 * whitespace around it) so the "done"/"jobs" output shows just the
 * command itself. */
static void copy_job_cmdline(const char *cmdline, char *out, size_t out_size) {
    strncpy(out, cmdline, out_size - 1);
    out[out_size - 1] = '\0';

    size_t len = strlen(out);
    while (len > 0 && (out[len - 1] == ' ' || out[len - 1] == '\t')) {
        len--;
    }
    if (len > 0 && out[len - 1] == '&') {
        len--;
    }
    while (len > 0 && (out[len - 1] == ' ' || out[len - 1] == '\t')) {
        len--;
    }
    out[len] = '\0';
}

int add_job(pid_t pid, const char *cmdline) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!job_table[i].active) {
            Job *job = &job_table[i];
            job->job_num = next_job_num++;
            job->pid = pid;
            copy_job_cmdline(cmdline, job->cmdline, sizeof(job->cmdline));
            job->active = 1;

            printf("[%d] %d\n", job->job_num, (int)job->pid);
            fflush(stdout);
            return job->job_num;
        }
    }

    /* Should not happen given the "at most 10 concurrent jobs"
     * assumption; the child still runs and is reaped silently below. */
    fprintf(stderr, "shell: job table full, not tracking pid %d\n", (int)pid);
    return -1;
}

void reap_finished_jobs(void) {
    /* waitpid(-1, ...) rather than per-job waits so that untracked
     * children (e.g. the earlier stages of a background pipeline, where
     * only the last stage's PID is recorded) are reaped too instead of
     * lingering as zombies. Foreground children have always been waited
     * for by the time this runs, so nothing else can be stolen here. */
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < MAX_JOBS; i++) {
            if (job_table[i].active && job_table[i].pid == pid) {
                printf("[%d]+ done %s\n", job_table[i].job_num,
                       job_table[i].cmdline);
                job_table[i].active = 0;
                break;
            }
        }
    }
    fflush(stdout);
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
