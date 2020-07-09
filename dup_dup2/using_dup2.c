#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	char buf[256] = {0};
	if (argc != 2)
	{
		printf("%s <filename>\n", *argv);
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("open failed\n");
		return -1;
	}

	int stdin_copy = dup(STDIN_FILENO); 	// keep stdin
	dup2(fd, STDIN_FILENO);			// STDIN_FILENO points to fd's
	read(STDIN_FILENO, buf, 256);		// Read file argv[1]
	dup2(stdin_copy, STDIN_FILENO);		// STDIN_FILENO points to stdin_copy

	buf[255] = 0;
	printf("%s\n", buf);
	close(fd);
}
