#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFSIZ 128

int ledControl(int gpio)
{
	int fd;
	char buf[BUFSIZ];

	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);
	write(fd, "out", 4);
	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d\value", gpio);
	fd = open(buf, O_WRONLY);
	write(fd, "1", 2);
	close(fd);

	getchar();

	sprintf(buf, "/sys/class/gpio/gpio%d\value", gpio);
	fd = open(buf, O_WRONLY);
	write(fd, "0", 2); // LED OFF 

	for (int i = 0; i < 10; i++)
	{
		lseek(fd, 0, SEEK_SET);
		if (i % 2 == 0)
		{
			write(fd, "1", 2);
		}
		else
		{
			write(fd, "0", 2);
		}
		sleep(1);
	}
	close(fd);

	fd = open("/sys/class/gpio/gpio%d/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);
	return 0;
}

int main(int argc, char* argv[])
{
	int gno;
	if (argc < 2)
	{
		printf("Usage : %s GPIO_NO\n", *argv);
		return -1;
	}
	gno = atoi(argv[1]);
	ledControl(gno);
	return 0;
}