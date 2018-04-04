#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
struct timeval tv1, tv2, dtv;
struct timezone tz;
void time_start()
{
	gettimeofday(&tv1, &tz);
} long time_stop()
{
	gettimeofday(&tv2, &tz);
	dtv.tv_sec = tv2.tv_sec - tv1.tv_sec;
	dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
	if (dtv.tv_usec < 0) {
		dtv.tv_sec--;
		dtv.tv_usec += 1000000;
	}
	return dtv.tv_sec * 1000000 + dtv.tv_usec;
}

int main(int argc, char *argv[], char *envp[])
{
	if (argc > 1) {
		char **argvnew;
		argvnew = argv + 1;
		int i, l;
		int r = fork();
		if (r != 0)
		{
			time_start();
			wait(NULL);
			printf("Time of working programs %s = %ld microseconds\n", argv[1], time_stop());
		}
		if (r == 0)
		{
			if ((l = execvp(argv[1], argvnew)) >= 0) {
			} else {
					perror(argv[1]);
			}	
		}
	}
	return 0;
}
