#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n;
    int total = 0;
    srand(clock());
    scanf("%d", &n);
    int fds[2];
    pipe(fds);

    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        if (pid != 0)
        {
            int ret;
            read(fds[0], &ret, sizeof(int));
            wait(NULL);
            total += ret;
        }
        else if (pid == 0)
        {
            int sum = 0;
            srand(clock());
            for (int j = 0; j < 10; j++)
            {
                sum += rand() % 10;
            }
            printf("[child no. %d] sum = %d\n", i, sum);
            write(fds[1], &sum, sizeof(int));
            return 0;
        }
    }
    printf("[parent] total_sum = %d\n", total);
}