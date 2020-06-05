#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

void printCount(char* file)
{
	pid_t pid;
	pid = fork();

	int status;
	if (!pid)
	{
		execlp("wc", "wc", file, NULL);	
	}
	waitpid(pid, &status, 0);	
}

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("usage : %s <filename> <filename2> ...)\n", argv[0]);
		return -1;
	}
	
	for (int i = 1; i < argc; i++)
	{
		printCount(argv[i]);
	}	
}
