#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <wctype.h>

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

int IS_WORD(char c){
	int ret = 0;

	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c == '-' || c == '.' || c == '_')
		ret = 1;
	
	return ret;
}

int main(int argc, char* argv[])
{
	int ret = 0;
	if (argc > 1) {
		int fd[2];
		size_t size;
		char string[4092];
		char **argvnew;
		argvnew = argv + 1;
		int i, l;
		if (pipe(fd) < 0)
			{	
				printf("Can't create pipe\n");
				exit(-1);
			}
		int r = fork();
		if (r != 0)
		{
			time_start();
			wait(NULL);
			printf("Time of working programs %s = %ld microseconds\n", argv[1], time_stop());
			int k;
			int strings = 0;
			int bytes = 0;
			int words = 0;
			close(fd[1]);
			int r = 0;
			int flag = 0;
			size_t length = 0;
			while((size = read(fd[0], string, sizeof(string) - 1)) > 0)
            		{
                		bytes = bytes + size;
				int i;
				for (i = 0; i < size; i++)
				{
					if (string[i] == '\n')
						strings++;
				}
				i = 0;
				if(size == -1)
				{
					perror(argv[0]);
					ret = 1;
					goto out;
				}
				string[size] = 0;
				length += size;
				for(i = 0; i < size; i++)
				{
					if(string[i] == '\n')
					{
						if(flag)
						{
							flag = 0;
							words++;
						}
					} 
					else
					{
						if(IS_WORD(string[i]))
						{
							if(!flag)
							{
								flag = 1;
							}
						}
						else 
						{
							if(flag)
							{
								flag = 0;
								words++;
							}
						}
					}
				}	
			}	
			printf("Было передано %d байт\n", bytes);
			printf("Слов - %d\n", words);
			printf("Строк - %d\n", strings);
			close(fd[0]); 
		}
		if (r == 0)
		{
			if (dup2(fd[1], 1) == -1)
				perror("dup2");
			if ((l = execvp(argv[1], argvnew)) >= 0) {
			} else {
					perror(argv[1]);
			}
			close(fd[1]);
			close(fd[0]);	
		}
	}
	out:
	return ret;
}
