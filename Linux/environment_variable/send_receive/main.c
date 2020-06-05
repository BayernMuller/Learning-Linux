#include <stdio.h>
#include <unistd.h>


int main()
{
	char* envlist[] = { "APPLE=0", "BANANA=1", NULL};
	execle("sub", "sub", NULL, envlist);
}
