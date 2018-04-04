#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
void Recursivels(char* q)
{
	DIR* dir = opendir(q);
	struct dirent* p;
	printf("./%s:\n", q);
        while (readdir(dir) != NULL)
        {
                printf("%s ", readdir(dir)->d_name);
        }
        int a = closedir(dir);
}
int main(int argc, char *argv[])
{
	Recursivels(argv[1]);
	return 0;
}
