#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int filedes[2];
    pipe(filedes);

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (rc == 0) {
        dup2(filedes[1], STDOUT_FILENO);
        printf("hello from child 1\n");
    } else {
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
        } else if (rc == 0) {
            dup2(filedes[0], STDIN_FILENO);

            char message[19];
            fgets(message, 18, stdin);

            printf("child 1 said: %s\n", message);
        } else {
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
