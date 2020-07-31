#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>

typedef struct 
{
	int a;
	char b[10];
} Form;

int main()
{
	int shmid;
	void *shared_memory = NULL;
	Form *form;
	shmid = shmget((key_t)1234, sizeof(form), 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		perror("shmget failed\n");
		exit(0);
	}

	shared_memory = shmat(shmid, NULL, 0);
	if (shared_memory == (void*)-1)
	{
		perror("shmat failed\n");
		exit(0);
	}
	form = (Form*)shared_memory;
        printf("shm data = %d, %s\n", form->a, form->b);
        return 0;
}
