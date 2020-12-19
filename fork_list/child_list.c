#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n, total;
    scanf("%d", &n);
    total = n;

    for (int i = 0; i < 2; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            i = 0;
            printf("[child %d] pid: %d / ppid: %d\n", total - n, getpid(), getppid());
            if (--n)
            {
                continue;
            }
            exit(0);
        }
        else if (pid > 0)
        {
            wait(NULL);
            break;
        }
    }
}