#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
void handler(int);

int main()
{
	struct sigaction act;
	sigset_t set;
	
	sigfillset(&act.sa_mask);
	act.sa_handler = handler;

	sigaction(SIGCHLD, &act, NULL);

	pid_t pid = fork();
	if (!pid)
	{
		sleep(2);
		//kill(getpid(), 9);
		exit(10);
	}
	else
	{
		printf("%ld\n", time(NULL));
		for(int k =0; k<5; k++)
		{
			sleep(1);
		}
		printf("%ld\n", time(NULL));
	}
}

void handler(int signum)
{
	int pid, status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		printf("SIGCHLD, %d %d\n", pid, signum);
		if (WIFEXITED(status))
		{
			printf("exit %d\n", WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			printf("signal %d\n", WTERMSIG(status));
		}
	}
}
