#include <stdio.h>
#include <unistd.h>

void OnDestroy1()
{
	printf("process is dying 1\n");
}

void OnDestroy2()
{
	printf("process is dying 2\n");
}

int main()
{
	atexit(OnDestroy1);
	atexit(OnDestroy2);
	exit(0);
}
