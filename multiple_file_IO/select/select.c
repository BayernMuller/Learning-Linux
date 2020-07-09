#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	struct timeval tv;
	fd_set readfds;
	int ret;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	tv.tv_sec = 5; // 5 second
	tv.tv_usec = 0;

	ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

	if (ret == -1)
	{
		perror("select");
		return -1;
	}
	else if (!ret)
	{
		printf("%d seconds elasped.\n", 5);
		return 0;
	}

	// if following codes execute, you can read fd immediately
	
	if (FD_ISSET(STDIN_FILENO, &readfds))
	{
		char buf[1024];
		int len;

		// this read function never block.
		len = read(STDIN_FILENO, buf, 1024);
		if (len == -1)
		{
			perror("read");
			return -1;
		}

		if (len)
		{
			buf[len] = 0;
			puts(buf);
		}
		return 0;
	}
	perror("This should not happen!\n");
	return -1;	
}
