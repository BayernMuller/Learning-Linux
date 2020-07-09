#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#define MAX_SW 10
#define MAX_STR 256

typedef struct tagInfo
{
	char info[4][MAX_STR];	// name and params
	char reason[MAX_STR];	// reason of death
	int count;		// init count
	time_t begin_time;	// process begin time
	pid_t pid;		// process ID
} Info;

Info set[MAX_SW];
int block_count;

void initChildren();
void setSignal();
void excuteChild(int idx);
void handler(int signum);
void writeBlock(FILE* pfile); // num is number of s/w blocks
int readBlock(char* path); // return value is number of s/w blocks;
int findIdx(pid_t pid);
int isToken(char ch);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("usage : %s <filename>\n", argv[0]);
		return -1;
	}
	
	initChildren();
	setSignal();
	block_count = readBlock(argv[1]);
	
	while (1)
	{
		for (int i = 0; i < block_count; i++)
		{
			if (set[i].pid == -1)
			{
				excuteChild(i);
			}
		}
		writeBlock(stdout);
		sleep(1);
		system("clear");
	}
}

void handler(int signum)
{
	int pid, status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		int idx = findIdx(pid);
		if (idx == -1)
			return;
		if (WIFEXITED(status))
		{
			sprintf(set[idx].reason, "EXIT(%d)", WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			sprintf(set[idx].reason, "SIGNAL(%d, %s)", WTERMSIG(status), strsignal(WTERMSIG(status)));
		}
		set[idx].pid = -1;
	}
}

int readBlock(char* path)
{
	int fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		printf("fail to open\n");
		exit(-1);
	}

	int sw_cnt = 0;
	char buf[MAX_SW * MAX_STR] = {0,}, *pt;
	if (read(fd, buf, MAX_SW * MAX_STR) < 0)
	{
		printf("fail to read\n");
		exit(-1);
	}

	pt = buf;
	for (sw_cnt = 0; sw_cnt < MAX_SW && *pt; sw_cnt++)
	{
		for (int j = 0; j < 4; j++)
		{
			sscanf(pt, "%s", set[sw_cnt].info[j]);
			pt += strlen(set[sw_cnt].info[j]);
			for (; *pt && isToken(*pt); pt++)
			{
				if (*pt == '\n')
					j = 0xFFFF;
				// break outside for loop
			}
		}
	}
	close(fd);
	return sw_cnt;
}

void writeBlock(FILE* pfile)
{
	char time_str[MAX_STR];
	fprintf(pfile, "%15s%15s%15s\t%s\n", "NAME", "INIT_COUNT", "INIT_TIME", "REASON");
	//fprintf(pfile, "===========================================================================\n");
	for (int i = 0; i < block_count; i++)
	{
		strftime(time_str, MAX_STR, "%H:%M:%S", localtime(&set[i].begin_time));
		fprintf(pfile, "%15s%15d%15s\t%s\n", set[i].info[0], set[i].count, time_str, set[i].reason);
	}
}

void excuteChild(int idx)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		char buf[MAX_STR];
		getcwd(buf, MAX_STR);
		strcat(buf, "/");
		strcat(buf, set[idx].info[0]);
		execl(buf, set[idx].info[0], set[idx].info[1], set[idx].info[2], set[idx].info[3], NULL);
		return;
	}
	set[idx].begin_time = time(NULL);
	set[idx].pid = pid;
	set[idx].count++;
}

int isToken(char ch)
{
	return ch == ' ' || ch == ';' || ch == '\n';
}

void initChildren()
{
	for (int i = 0; i < MAX_SW; i++)
	{
		strcpy(set[i].reason, "Init.");
		set[i].pid = -1;
	}
}

void setSignal()
{
	static struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&(act.sa_mask));
	sigaddset(&(act.sa_mask), SIGCHLD);
	sigaction(SIGCHLD, &act, NULL);
}

int findIdx(pid_t pid)
{
	for (int i = 0; i < block_count; i++)
	{
		if (set[i].pid == pid)
		{
			return i;
		}
	}
	return -1;
}
