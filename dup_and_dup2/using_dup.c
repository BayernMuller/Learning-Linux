#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int fd = dup(STDOUT_FILENO); // copy file descriptor
	
	printf("stdout : %d\nfd : %d\n", STDOUT_FILENO, fd);
	
	char* str = "Lewandowski is better than Benzema\n";
	int length = 0;
	while (str[length++]);
	write(fd, str, length); // print to stdout.
	return 0;
}
