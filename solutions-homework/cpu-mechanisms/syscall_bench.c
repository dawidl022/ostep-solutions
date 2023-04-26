#include <stdio.h>
#include <sys/time.h>

void basicSyscall()
{
    fopen("test.txt", "w");
}

long benchmark(void (*func)())
{
    struct timeval start;
    gettimeofday(&start, NULL);

    const int iterations = 10000;

    for (int i = 0; i < iterations; i++) {
        func();
    }

    struct timeval end;
    gettimeofday(&end, NULL);

    int endMicros = end.tv_sec * 1000 * 1000 + end.tv_usec;
    int startMicros = start.tv_sec * 1000 * 1000 + start.tv_usec;

    return (endMicros - startMicros) / iterations;
}

int main(int argc, char* argv[])
{
    printf("%ldμs\n", benchmark(&basicSyscall));

    return 0;
}
