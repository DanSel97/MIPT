#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char* argv[])
{
	if (strcmp(argv[1],"1") == 0)
	{
		int i = 0;
		printf("PID = %d\n", (int)getpid());
		pid_t pid = 0;
		while(!(pid = fork()) && i < 5)
		{
			printf ("i = %d, fork() = %d\n", i, (int)pid);
			printf("PID = %d, PID parent = %d\n\n", (int)getpid(), (int)getppid());
			i++;
		}
		if (i != 5)
			wait(NULL);
		exit(0);
	}

	if (strcmp(argv[1],"2") == 0)
	{
		pid_t* children = (pid_t*) calloc (sizeof(pid_t), 5);
		pid_t new_pid = 0;
		int i;
		for (i = 0; i < 5; i++)
		{
			new_pid = fork();
			if (new_pid == 0)
				break;
			else
				children[i] = new_pid;
		}
		printf("My PID = %d, my parent PID = %d\n", (int)getpid(), (int)getppid());
		if (new_pid) {
			for (i = 0; i < 5; i++)
				wait(NULL);
			for (i = 0; i < 5; i++)
				printf("Child[%d] PID = %d\n", i, (int)children[i]);
		}
		free(children);
	}
	return 0;
}
