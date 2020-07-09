#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	printf("Hello!\n");

	pid = fork();

	if (pid > 0)
	{
		printf("I am parent\n");
		sleep(1);
	}
	else if (!pid)
	{
		printf("I am child\n");
		execl("/bin/ls", "ls", "-l", NULL);
		printf("fail to execute\n");
	}
	else
	{
		printf("fail to fork()\n");
	}
	printf("Bye!\n");
	return 0;
}

