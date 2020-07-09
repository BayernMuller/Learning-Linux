#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	sigset_t set;
	int result;

	sigemptyset(&set);
	result = sigismember(&set, SIGALRM);

	printf("SIGNALRM is %sa member\n", result ? "" : "not ");
	sigaddset(&set, SIGALRM);

	result = sigismember(&set, SIGALRM);
	printf("SIGALRM is %sa member\n", result ? "" : "not ");

	sigfillset(&set);

	result = sigismember(&set, SIGCHLD);
	printf("SIGCHLD is %sa member\n", result ? "" : "not ");
	sigdelset(&set, SIGCHLD);

	result = sigismember(&set, SIGCHLD);
	printf("SIGCHLD is %sa member\n", result ? "" : "not ");
}




