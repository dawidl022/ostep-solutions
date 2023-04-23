#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (rc == 0) {
        close(STDOUT_FILENO);
        printf("hello\n"); // has no effect since stdout is closed
    } else {
        int child_status;
        int child_pid = wait(&child_status);
        printf("child with PID %d return with status %d\n",
            child_pid, child_status);
    }

    return 0;
}
