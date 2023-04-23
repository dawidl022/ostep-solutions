#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    char* env[] = { "LS_COLWIDTHS=10:10:10", NULL };
    char* args[] = { "ls", "-l", NULL };

    for (int i = 0; i < 6; i++) {
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr, "fork failed\n");
        } else if (rc == 0) {
            int exec_failed = 0;

            // many variants of exec exist to offer flexibility

            // after experimenting, I identified the following "flags" for exec:
            // l - args as list of function arguments
            // v - args as array
            // p - PATH resolution performed
            // e - environment variable array included

            switch (i) {
            case 0:
                // the interesting thing is that the first argument to the
                // program (second arg to exec) can be anything...
                exec_failed = execl("/bin/ls", "ls", "-l", NULL);
                break;
            case 1:
                exec_failed = execle("/bin/ls", "ls", "-l", NULL, env);
                break;

            case 2:
                exec_failed = execlp("ls", "ls", "-l", NULL);
                break;

            case 3:
                exec_failed = execv("/bin/ls", args);
                break;

            case 4:
                exec_failed = execvp("ls", args);
                break;

            case 5:
                // execvpe is a GNU extension (not POSIX) not supported on macOS  
                exec_failed = execve("/bin/ls", args, env);
                break;
            }

            if (exec_failed) {
                fprintf(stderr, "failed to exec\n");
            }
            return exec_failed;
        } else {
            wait(NULL);
        }
    }


    return 0;
}
