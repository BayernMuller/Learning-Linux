#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define LOGDIR "./AppLog"
#define LOGFILE "mylog"

char *gettime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
		
	time_t t = (time_t) tv.tv_sec;
	struct tm *ptm = localtime(&t);
	static char str[1024];

	sprintf(str, "%04d.%02d.%02d %02d:%02d:%02d.%03d ",
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, 
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (int)tv.tv_usec/1000);
	return str;
}

int main(int argc, char **argv)
{	
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	int ran = random();
	
	mkdir(LOGDIR, 0755);
	chdir(LOGDIR);

	FILE *fp;
	fp = fopen(LOGFILE, "a");
	fprintf(fp, "%s %s sleep %d\n", gettime(), argv[0], ran%20);
	fclose(fp);
	sleep(ran%20+1);

	if(ran % 2)	//odd => signal
	{
		int signum = ((ran/2)%15+1);
		FILE *fp;
		fp = fopen(LOGFILE, "a");
		fprintf(fp, "%s ", gettime());
		for(int k=0; k<argc; k++)
			fprintf(fp, "%s ", argv[k]);
		fprintf(fp, "\n");
		fprintf(fp, "%s %s SIGNAL(%d).%s\n", 
				gettime(), argv[0], signum, strsignal(signum));
		fclose(fp);
		kill(getpid(), signum);
	}
	else // even -> exit
	{
		int exitval = (ran/2)%256;
        FILE *fp;
        fp = fopen(LOGFILE, "a");
		fprintf(fp, "%s ", gettime());
		for(int k=0; k<argc; k++)
			fprintf(fp, "%s ", argv[k]);
		fprintf(fp, "\n");
		fprintf(fp, "%s %s EXIT(%d)\n", 
				gettime(), argv[0], exitval);
		fclose(fp);
		exit(exitval);
	}
}
