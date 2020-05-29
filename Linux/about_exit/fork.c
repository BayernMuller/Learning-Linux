#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	int i = 0;

	i++;

	printf("before calling fork(%d)\n", i);

	pid = fork();

	if (pid == 0) // child process
	{
		printf("child process(%d)\n", ++i);
	}
	else if (pid > 0) // parent process
	{
		printf("parent process(%d)\n", --i);
	}
	else
	{
		printf("fail to fork()\n");
	}
	return 0;
}
