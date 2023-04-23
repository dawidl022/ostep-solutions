#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (rc == 0) {
        printf("hello\n");
    } else {
        // according to the man pages:
        // wait(&status) is equivalent to waitpid(-1, &status, 0)

        // waitpid is useful when we need more control over what to wait for
        // via the pid argument (1st arg) and options argument (3rd arg)

        int child_pid = waitpid(-1, NULL, 0);
        printf("goodbye %d\n", child_pid);
    }

    return 0;
}
