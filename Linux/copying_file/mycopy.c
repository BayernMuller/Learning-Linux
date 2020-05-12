#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int copy(const char* src, const char* dst)
{
	int fdin = open(src, O_RDONLY);
	int fdout = open(dst, O_WRONLY | O_CREAT, 0644);
	int bytes = 0;
	
	char buf[1024] = {0};
	while ((bytes = read(fdin, buf, 1024)) > 0)
	{
		write(fdout, buf, bytes);	
	}
	close(fdin);
	close(fdout);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		return -1;
	}
	copy(argv[1], argv[2]);
}
