#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>

typedef struct
{
	int a;
	char b[10];
} Form;

#define SHM_KEY 0x00001234
#define SEM_KEY 0x00001234

struct sembuf lock_op[2] = {0,0,0,0,1,0};
struct sembuf unlock_op[1] = {0, -1, 0};
int semid;
void lock() { semop(semid, lock_op, 2); }
void unlock() { semop(semid, unlock_op, 1); }

int main()
{
	int shmid;
	void* shared_memory = NULL;
	Form *form;

	shmid = shmget((key_t)SHM_KEY, sizeof(Form), 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		perror("shmget failed\n");
		return 0;
	}

	shared_memory = shmat(shmid, NULL, 0);
	if (shared_memory == (void*)-1)
	{
		perror("shmat failed\n");
		return 0;
	}

	form = (Form*)shared_memory;
	semid = semget((key_t)SEM_KEY, 1, 0666 | IPC_CREAT);
	if (semid == -1)
	{
		perror("semget failed\n");
		return 0;
	}

	semctl(semid, 0, SETVAL);
	lock();

#ifdef __CLIENT__
	printf("%d %s\n", form->a, form->b);
#endif
#ifdef __SHM__
	form->a = 3;
	memcpy(form->b, "test", sizeof(form->b));
	getchar();
#endif
	unlock();
	return 0;
}
