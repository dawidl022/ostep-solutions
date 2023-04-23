#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    // both parent and child are able to write to this file descriptor concurrently
    FILE* fptr = fopen("file.txt", "w");

    if (fptr == NULL) {
        fprintf(stderr, "failed to open file \"file.txt\"");
        return 1;
    }

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        return 1;
    } else if (rc == 0) {
        fprintf(fptr, "Writing to file from child process\n");
    } else {
        fprintf(fptr, "Writing to file from parent process\n");
    }

    return 0;
}
