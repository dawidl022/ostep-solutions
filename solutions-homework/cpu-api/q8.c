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
        dprintf(filedes[1], "hello from child 1");
    } else {
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
        } else if (rc == 0) {
            char message[19];
            read(filedes[0], message, 18);
            message[18] = '\0';

            printf("child 1 said: %s\n", message);
        }

        wait(NULL);
        wait(NULL);
    }



    return 0;
}
