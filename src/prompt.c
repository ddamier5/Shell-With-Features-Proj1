#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prompt.h"
#include "shell.h"

/* Part 1: Prompt (Ammiel Bowen, lead). */

void prompt_init(void) {
    if (getenv("USER") == NULL) {
        struct passwd *pw = getpwuid(getuid());
        setenv("USER", pw != NULL ? pw->pw_name : "user", 1);
    }

    if (getenv("MACHINE") == NULL) {
        char host[256];
        if (gethostname(host, sizeof(host)) != 0) {
            strncpy(host, "machine", sizeof(host) - 1);
            host[sizeof(host) - 1] = '\0';
        }
        setenv("MACHINE", host, 1);
    }

    if (getenv("PWD") == NULL) {
        char cwd[MAX_PATH_LEN];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            setenv("PWD", cwd, 1);
        }
    }
}

void print_prompt(void) {
    const char *user = getenv("USER");
    const char *machine = getenv("MACHINE");
    const char *pwd = getenv("PWD");

    printf("%s@%s:%s> ",
           user != NULL ? user : "user",
           machine != NULL ? machine : "machine",
           pwd != NULL ? pwd : "?");
    fflush(stdout);
}
