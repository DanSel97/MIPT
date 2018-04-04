#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[])
{
	int i;
	if (strcmp(argv[1], "-n") == 0)
	{
		for (i = 2; i < argc - 1; i++)
		{
			printf("%s ", argv[i]);
		}
		printf("%s", argv[argc - 1]);
	}
	else
	{
		for (i = 1; i < argc; i++)
                {
                        printf("%s ", argv[i]);
                }
		printf("\n");
	}
	return 0;
}
