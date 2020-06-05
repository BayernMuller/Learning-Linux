#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	putenv("APPLE=BANANA");
	printf("%s\n", getenv("APPLE"));

	execl("ex0811", "ex0811", NULL);
}
