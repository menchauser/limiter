#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

void showError(const char *funcName) {
    fprintf(stderr, "%s failed: %s\n", funcName, strerror(errno));
    exit(1);
}

int main(int argc, char **argv) {
    int c;
    extern char *optarg;
    extern int optind;

    int usecs = 0;
    int mem = 0;

    char *path;
    
    while ((c = getopt(argc, argv, "t:m:h")) != -1) {
        switch(c) {
            case 'h':
                printf("Usage: %s [-t milliseconds] [-m kilobytes] <executable>\n", argv[0]);
                exit(0);
            case 't':
                usecs = strtol(optarg, (char **)NULL, 10)*1000;
                if (errno < 0) showError("strtol()");
                break;
            case 'm':
                mem = strtol(optarg, (char **)NULL, 10)*1024;
                if (errno < 0) showError("strtol()");
                break;
            default:
                break;
        } 
    }
    
    if (optind == argc) {
        printf("Please, specify path to executable\n");
        printf("Use -h option for usage\n");
        exit(0);
    }
    path = argv[optind];

    pid_t child = fork();
    if (child < 0) {
        //error
        showError("fork()");
    } else if (child == 0) {
        //child
        printf("setting memory limit...");
        struct rlimit rlim;
        getrlimit(RLIMIT_AS, &rlim);
        rlim.rlim_cur = mem;
        if (setrlimit(RLIMIT_AS, &rlim) == -1) showError("setrlimit()");
        printf("trying to run %s...\n", path);
        execvp(path, argv + 2);
        //if running failed
        showError("execv()");
    } else {
        //parent
        printf("sleeping for %d usecs\n", usecs);
        usleep(usecs);
        printf("killing child %d\n", child);
        if (kill(child, 9) < 0) {
            showError("kill()");
        } else {
            printf("child process succesfully ended\n");
        }
        printf("parent process succesfully ended\n");
    }

    return 0;
}
