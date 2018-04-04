#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/sem.h>

#define N 5
void Judge(int A, int B, int C, int D, int E)
{
	sleep(1);
	struct mymsgbuf
	{
		long mtype;
		char mtext[80];
	} mybuf;
	int i;
	for (i = 1; i <= N; i++)
	{
		mybuf.mtype = 1;
		msgsnd(A, (struct msgbuf*) &mybuf, 0, 0);//send(A, msg);
	}
	printf("Я судья, пришёл проводить забег и начинаю раздавать карточки\n");
	fflush(stdout);
	for (i = 1; i <= N; i++)
	{
		mybuf.mtype = 1;
		msgsnd(E, (struct msgbuf*) &mybuf, 0, 0);//send(B, msg);
	}
	for (i = 1; i <= N; i++)
	{
		msgrcv(B, (struct msgbuf*) &mybuf, 0, 0, 0);//receive(B, msg);
	}
	printf("Все бегуны на месте, начинаю забег\n");
	fflush(stdout);
	mybuf.mtype = 1;
	msgsnd(C, (struct msgbuf*) &mybuf, 0, 0); //send(C[1], msg);
	msgrcv(C, (struct msgbuf*) &mybuf, 0, N + 1, 0); //receive(C[N + 1], msg);
	printf("Забег провёл, ухожу\n");
	fflush(stdout);
	for (i = 1; i <= N; i++)
	{
		mybuf.mtype = 1;
		msgsnd(D, (struct msgbuf*) &mybuf, 0, 0); //send(D, msg);
	}
}
void Runner(int A, int B, int C, int D, int E, long i)
{
	if (i == 1)
		sleep(3);
	struct mymsgbuf
	{
		long mtype;
		char mtext[80];
	} mybuf;
	printf("Я бегун %ld, пришёл бегать, жду судью\n", i);
	fflush(stdout);
	int k = 1;
	while (msgrcv(A, (struct msgbuf*) &mybuf, 0, 0, IPC_NOWAIT) == -1 && errno == ENOMSG)
	{
			printf("Я бегун %ld, судью пока не встретил, решил побегать на стадионе, потренироваться там, бегаю %d круг\n", i, k);
			fflush(stdout);
			sleep(1);
			k++;
	}//receive(A, msg);
	msgrcv(E, (struct msgbuf*) &mybuf, 0, 0, 0);
	printf("Я бегун %ld, взял карточку, жду начала забега\n", i);
	fflush(stdout);
	mybuf.mtype = 1;
	msgsnd(B, (struct msgbuf*) &mybuf, 0, 0);//send(B, msg);y
	msgrcv(C, (struct msgbuf*) &mybuf, 0, i, 0);//receive(C[i], msg);
	printf("Я бегун %ld, взял палку, начинаю забег\n", i);
	fflush(stdout);
	if (i != N)
	{
		printf("Я бегун %ld, побегал, передаю эстафету следующему\n", i);
		fflush(stdout);
		mybuf.mtype = i + 1;
		msgsnd(C, (struct msgbuf*) &mybuf, 0, 0);//send(C[i + 1], msg);
		printf("Я бегун %ld, жду окончания забега\n", i);
		fflush(stdout);
		msgrcv(D, (struct msgbuf*) &mybuf, 0, 0, 0); //receive(D, msg);
		printf("Я бегун %ld, побегал, попрощался с судьёй, ухожу\n", i);
		fflush(stdout);
	}
	else 
	{
		printf("Я бегун %ld, побегал, передаю палку судье\n", i);
		fflush(stdout);
		mybuf.mtype = i + 1;
		msgsnd(C, (struct msgbuf*) &mybuf, 0, 0);//send(C[i + 1], msg);
		msgrcv(D, (struct msgbuf*) &mybuf, 0, 0, 0); //receive(D, msg);
		printf("Я бегун %ld, побегал, попрощался с судьёй, ухожу\n", i);
		fflush(stdout);
	}
}
int main(int argc, char** argv)
{
		int A  = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
		if (A == -1) {
				perror("MSGGET A");
				return -1;
		   }
		int B  = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
		if (B == -1) {
				perror("MSGGET B");
				return -1;
		   }
		int C  = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
		if (C == -1) {
				perror("MSGGET C");
				return -1;
		   }
		int D  = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
		if (D == -1) {
				perror("MSGGET D");
				return -1;
		   }
		int E  = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666);
		if (E == -1) {
				perror("MSGGET D");
				return -1;
		   }

		pid_t cur_pid = -1;
		pid_t process[N + 1] = {};
		long i;
		for (i = 0; i <= N; i++) {
			cur_pid = fork();
			if (cur_pid == 0) {
				if (i == N)	(Judge(A, B, C, D, E));
				else Runner(A, B, C, D, E, i + 1);
				break;
			}
			else {
				process[i] = cur_pid;
			}
		}
		if (cur_pid != 0) {
			int runned = N + 1;
			int status;
			while (runned > 0) {
				pid_t wpid = wait(&status);
				if (WIFEXITED(status))
					runned--;
			}
			msgctl(A, IPC_RMID, NULL);
			msgctl(B, IPC_RMID, NULL);
			msgctl(C, IPC_RMID, NULL);
			msgctl(D, IPC_RMID, NULL);
		}
	return 0;
}
