#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	char* file = (char*)malloc(256 * sizeof(char));
	sprintf(file,"newdir/%d", (int)getpid());

	int hello = mkdir("newdir", 0777);
	int f = open(file, O_CREAT|O_EXCL);
	if (f < 0)
	{
		hello = mkdir("newdir", 0770);
		f = open(file, O_CREAT|O_EXCL);
	}
	if(f >= 0)
		close(f);

	int len = 0;
	char str[12];

	if(!hello)
	{
		sprintf(str, "Hello world\n");
		len = 12;
	}
	else
	{
		sprintf(str, "Bye world\n");
		len = 10;
	}

	int i = 0;

	for(i = 0; i < len; i++)
	{
		char s[1];
		s[0] = str[i];
		write(1, s, 1);
		sleep(1);
	}
	unlink(file);
	rmdir("newdir");

	
	return 0;
}	
