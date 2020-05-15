#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define NAME_MAX 16

struct student
{
	char name[NAME_MAX];
	int roomNo;
};

void usage(char* cmd)
{
	printf("usage : %s option(0/1) name roomNo name roomNo ... \n", cmd);
}

void writeAsciiFile(int count, struct student* pSp)
{
	int fd = open("AsciiFile", O_RDWR | O_CREAT, 0644);
	char buf[1024];
	for (int i = 0; i < count; i++)
	{
		memset(buf, 0, 1024);
		sprintf(buf, "%s, %d\n", pSp[i].name, pSp[i].roomNo);
		write(fd, buf, strlen(buf));
	}
	close(fd);
}

void writeBinaryFile(int count, struct student *pSp)
{
	int fd = open("BinaryFile", O_RDWR | O_CREAT, 0644);
	write(fd, pSp, sizeof(struct student) * count);
	close(fd);
}

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		usage(argv[0]);
		return -1;
	}

	int option = atoi(argv[1]);
	int count = (argc - 2) / 2;
	
	struct student myS[count];
	for (int i = 0; i < count; i++)
	{
		strcpy(myS[i].name, argv[2 * i + 2]);
		myS[i].roomNo = atoi(argv[2 * i + 2 + 1]);
	}

	if (option)
	{
		writeBinaryFile(count, myS);
	}
	else
	{
		writeAsciiFile(count, myS);
	}
}




