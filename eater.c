#include <stdio.h>
#include <stdlib.h>

#define SIZE 64*1024*1024 //64 MB

int main(int argc, char **argv) {
    void *mem;
    mem = malloc(SIZE);

    if (mem) {
        printf("Allocating %d bytes successfull!\n", SIZE);
    } else {
        printf("Error allocating %d bytes!\n", SIZE);
    }

    free(mem);

    return 0;
}
