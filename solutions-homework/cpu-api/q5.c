#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (rc == 0) {
        // child has no children of its own, so wait returns immediately with -1
        int no_pid = wait(NULL);
        printf("hello %d\n", no_pid);
    } else {
        // wait returns the PID of the process it was waiting for
        int child_pid = wait(NULL);
        printf("goodbye %d\n", child_pid);
    }

    return 0;
}
