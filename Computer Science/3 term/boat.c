#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include <errno.h>

#define PASS 20
#define BOAT_SIZE 5
#define K 5

#define SEM_PASSENGERS_ON_EARTH 0
#define SEM_SOMEONE_IN 1
#define SEM_SOMEONE_OUT 2
#define SEM_SWIM 3
#define SEM_TRAP_DOWN 4
#define SEM_WAIT_ALL 5
#define SEM_WAY_END 6
#define SEM_PASSENGERS_OUT 7

#define SET(command, opt_num, opt_op, opt_flg) \
command.sem_num = opt_num; \
command.sem_op = opt_op; \
command.sem_flg = opt_flg; 

#define SETSEM(SEM_NAME, VALUE) \
	if (semctl(semid, SEM_NAME, SETVAL, VALUE) == -1) perror("SETVAL #__LINE__");
#define V(SEM_NAME) \
	{ struct sembuf command = {SEM_NAME, 1, 0}; if (semop(semid, &command, 1) == -1) perror("SEM V " "#__LINE__"); }
#define P(SEM_NAME) \
	{ struct sembuf command = {SEM_NAME, -1, 0}; if (semop(semid, &command, 1) == -1) perror("SEM P " "#__LINE__"); }
#define Z(SEM_NAME) \
	{ struct sembuf command = {SEM_NAME, 0, 0}; if (semop(semid, &command, 1) == -1) perror("SEM WAIT " "#__LINE__"); }
#define VK(SEM_NAME, K) \
	{ struct sembuf command = {SEM_NAME, K, 0}; if (semop(semid, &command, 1) == -1) perror("SEM VK " "#__LINE__"); }
void pass(int semid, int pass_id)
{
	struct sembuf command1 = {SEM_WAY_END, 0, IPC_NOWAIT};
	if (semop(semid, &command1, 1) == 0)
	{
			printf("[%d] Я пришёл кататься\n", pass_id);
			printf("[%d] Накатался, ухожу\n", pass_id);
			exit(0);
	}
	
	printf("[%d] Я пришёл кататься\n", pass_id);
	while (1) {
		P(SEM_SOMEONE_IN);
		struct sembuf command1 = {SEM_WAY_END, 0, IPC_NOWAIT};
		if (semop(semid, &command1, 1) == 0)
		{
			printf("[%d] Накатался, ухожу\n", pass_id);
			exit(0);
		}
		printf("[%d] Иду по трапу на борт\n", pass_id);
		V(SEM_WAIT_ALL);
		printf("[%d] На борту\n", pass_id);
		P(SEM_PASSENGERS_ON_EARTH);
		struct sembuf command2 = {SEM_PASSENGERS_ON_EARTH, 0, IPC_NOWAIT};
		if (semop(semid, &command2, 1) == -1)
		{
			if (errno == EAGAIN)
			{
				V(SEM_SOMEONE_IN);
			}
		}
		Z(SEM_SWIM);
		printf("[%d] Я катаюсь!\n", pass_id);
		P(SEM_TRAP_DOWN);
		Z(SEM_PASSENGERS_OUT);
		V(SEM_TRAP_DOWN);
		P(SEM_SOMEONE_OUT);
		printf("[%d] Выхожу на берег\n", pass_id);
		P(SEM_WAIT_ALL);
		V(SEM_PASSENGERS_ON_EARTH);
		struct sembuf command3 = {SEM_WAIT_ALL, 0, IPC_NOWAIT};
		if (semop(semid, &command3, 1) == -1)
		{
			V(SEM_SOMEONE_OUT);
		}
	}
}

void boat(int semid)
{
		struct sembuf command;
		printf("[Лодка] Начинаем кататься\n");
		int ii;
		for (ii = K; ii > 0; ii--)
		{
			printf("[Лодка] Трап открыт, пассажиры заходят\n");
			V(SEM_SOMEONE_IN);
			Z(SEM_PASSENGERS_ON_EARTH);
			printf("[Лодка] Трап поднял, катаю\n");
			P(SEM_SWIM);
			Z(SEM_TRAP_DOWN);
			printf("[Лодка] Покатал, опускаю трап, пассажиры выходят\n");
			P(SEM_PASSENGERS_OUT);
			V(SEM_SWIM);
			V(SEM_SOMEONE_OUT);
			Z(SEM_WAIT_ALL);
			V(SEM_PASSENGERS_OUT);
		}

		printf("[Лодка] На сегодня всех покатал\n");
		P(SEM_WAY_END);
		for (ii = 0; ii < PASS; ii++)
		{
			V(SEM_SOMEONE_IN);
		}

}

int main(int argc, char** argv)
{	
		int semid = semget(IPC_PRIVATE, 8, IPC_CREAT | IPC_EXCL | 0666);
		if (semid == -1) {
				perror("SEMGET");
				return -1;
		   }
		VK(SEM_PASSENGERS_ON_EARTH, BOAT_SIZE);
		V(SEM_SWIM);
		VK(SEM_TRAP_DOWN, BOAT_SIZE);
		V(SEM_PASSENGERS_OUT);
		V(SEM_WAY_END);
		pid_t cur_pid = -1;
		pid_t process[pass + 1] = {};
		int i;
		for (i = 0; i <= pass; i++) {
			cur_pid = fork();
			if (cur_pid == 0) {
				if (i == 0)	boat(semid);
				else pass(semid, i);
				break;
			}
			else {
				process[i] = cur_pid;
			}
		}
		if (cur_pid != 0) {
			int runned = pass + 1;
			int status;
			while (runned > 0) {
				pid_t wpid = wait(&status);
				if (wifexited(status))
					runned--;
			}
			semctl(semid, IPC_RMID, 0);
		}
		return 0;
}
