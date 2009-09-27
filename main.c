#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int usecs;
    char *path;

    

    if (argc < 3) {
        fprintf(stderr, "usage: %s <milliseconds> <exec-file>\n", argv[0]);
        exit(1);
    }

    usecs = strtol(argv[1], (char **)NULL, 10)*1000;
    if (errno < 0) {
        fprintf(stderr, "%s: usleep() failed: %s\n", argv[0], strerror(errno));
        exit(1); 
    }
    path = argv[2];

    pid_t child = fork();
    if (child < 0) {
        //error
        fprintf(stderr, "%s: fork() failed: %s\n", argv[0], strerror(errno));
        exit(1); 
    } else if (child == 0) {
        //child 
        printf("trying to run %s...\n", path);
        execvp(path, argv + 2);
        //if running failed
        fprintf(stderr, "%s: execv() failed: %s\n", argv[0], strerror(errno));
        exit(1);
    } else {
        //parent
        printf("sleeping for %d usecs\n", usecs);
        usleep(usecs);
        printf("killing child %d\n", child);
        if (kill(child, 9) < 0) {
            fprintf(stderr, "%s: kill() failed: %s\n", argv[0], strerror(errno));
            exit(1);
        } else {
            printf("child process succesfully ended\n");
        }
        printf("parent process succesfully ended\n");
    }

    return 0;
}
