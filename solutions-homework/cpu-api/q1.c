#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int num = 100;

    printf("Starting value: %d\n", num);

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (rc == 0) {
        // child process has own copy of variable num
        printf("Value in child: %d\n", num);
        num = 50;
        printf("Updated value in child: %d\n", num);
    } else {
        // parent process
        num = 25;
        printf("Updated value in parent: %d\n", num);
    }

    return 0;
}
