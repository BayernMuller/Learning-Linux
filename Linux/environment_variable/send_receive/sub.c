#include <unistd.h>
#include <stdio.h>

extern char **environ;

int main()
{
	while (*environ)
		printf("%s\n", *environ++);
}
