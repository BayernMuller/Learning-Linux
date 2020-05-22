#include <sys/time.h>
#include <stdio.h>
#include <time.h>

void timedisplay();
void timedisplay2();

int main()
{
	timedisplay();
	timedisplay2();
}

void timedisplay()
{
	time_t t = time(NULL);
	printf("%ld\n", t);
	printf("%s\n", ctime(&t));

	struct tm *ptm = gmtime(&t);
	printf("%d %d %d %d %d %d\n", ptm->tm_year, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	ptm = localtime(&t);
	printf("%d %d %d %d %d %d\n", ptm->tm_year, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

void timedisplay2()
{
	struct timeval tv;
	struct timezone tz;

	printf("\n");
	printf("%d %d\n", (int)tv.tv_sec, (int)tv.tv_usec);
	printf("%d %d\n", (int)tz.tz_minuteswest, (int)tz.tz_dsttime);
}
