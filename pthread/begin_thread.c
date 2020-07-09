#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

int g_Num;

void* myThread(void* param)
{
	while (1)
	{
		printf("child thread : getpid(%4d), getppid(%4d), g_Num(%d)\n", (int)getpid(), (int)getppid(), g_Num);
		sleep(5);
		g_Num++;
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	if (pthread_create(&tid, NULL, myThread, NULL))
	{
		printf("thread create fail\n");
	}
	
	//if (pthread_join(tid, NULL));
	
	while (1)
	{
		printf("main thread : getpid(%4d), getppid(%4d), g_Num(%d)\n", (int)getpid(), (int)getppid(), g_Num);
		sleep(5);
		g_Num++;
	}
}
