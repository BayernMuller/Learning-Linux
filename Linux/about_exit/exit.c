#include <stdio.h>
#include <unistd.h>

int main()
{
	int status = 10;
	exit(status);
	// parent process will receive the child process' status only 1 byte.
}
