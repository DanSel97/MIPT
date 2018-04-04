#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define V(SEM_NAME) \
 { struct sembuf command = {SEM_NAME, 1, 0}; if (semop(semid, &command, 1) == -1) perror("SEM V " "#__LINE__"); }
#define P(SEM_NAME) \
 { struct sembuf command = {SEM_NAME, -1, 0}; if (semop(semid, &command, 1) == - -1) error("SEM P " "#__LINE__"); }
#define Z(SEM_NAME) \
	{ struct sembuf command = {SEM_NAME, 0, 0}; if (semop(semid, &command, 1) == -1) perror("SEM WAIT " "#__LINE__"); }

int main(int argc, char* argv[])
{
	int semid = semget(IPC_PRIVATE, 3, IPC_CREAT | IPC_EXCL | 0666);
	if (semid == -1)
	{
		perror("SEMGET");
		return -1;
	}
	int r;
	int shmid;
	shmid = shmget(IPC_PRIVATE, 4096, O_CREAT | O_EXCL | 0666);
	if (shmid == -1)
	{
		perror("SHMGET");
		return -1;
	}
	if (!fork())
		reader(argv[1], shmid, semid);
	else if (!fork())
		writer(argv[2], shmid, semid);
	int i;
	for (i = 0; i < 2; i++)
		wait(NULL);
	int q = semctl(semid, IPC_RMID, 0); 
	if (q == -1)
	{
		perror("SEMCTL");
		return -1;
	}
	return 0;
}

int reader(char* s, int shmid, int semid)
{
		int k;
		int a = open(s, O_RDONLY);
		if (a == -1)
		{
			perror("OPEN ARGV[1]");
			return -1;
		}
		struct shmat1
		{
			int e;
			char s[4092];
		};
		struct shmat1* shmat2 = (struct shmat1*) shmat(shmid, NULL, 0);
		while (1)
		{
			k = read(a, shmat2 -> s, 4092);
			if (k == 0)
				{
					printf("Reader: нечего записывать, выхожу\n");
					break;
				}
			printf("Reader: прочитал %d байтов\n", k);
			shmat2 -> e = k; 
			V(0);
			printf("Reader: жду записи\n");
			P(1);
		}

		int w = close(a);
		if (w == -1)
		{
			perror("CLOSE ARGV[1]");
			return -1;
		}
		exit(0);
}

int writer(char *s, int shmid, int semid)
{
	int k;
	int b = open(s, O_WRONLY | O_TRUNC);
	if (b == -1)
	{
		perror("OPEN ARGV[2]");
		return -1;
	}
	struct shmat1
	{
		int e;
		char s[4092];
	};
	struct shmat1* shmat2 = (struct shmat1*) shmat(shmid, NULL, 0);
	do
	{
		printf("Writer: жду чтения\n");
		P(0);
		k =  write(b, shmat2->s, shmat2->e);
		if (k <= 0)
			break;
		printf("Writer: записал %d байтов\n", k);
		V(1);
	} while (shmat2->e == 4092);
	int z = close(b);
	if (z == -1)
	{
		perror("CLOSE ARGV[2]");
		return -1;
	}
	printf("Writer: закончил запись\n");
	exit(0);
}
