#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char buf[4096];
	int k;
	int u;
	if (argc == 1)
	{ 
		while((k = read(0, buf, sizeof(buf))) > 0)
		{
			u = write(1, buf, k);
			if (u == -1)
			{
				char cat_str[] = "cat: ";
                perror(cat_str);
			}
		}
	}
	else
	{
		int i;
		int r;
		for (i = 1; i < argc; i++)
		{
			r = open(argv[i], O_RDONLY);
			if (r == -1)
			{
				char cat_str[] = "cat: ";
				char buffer[1000] = {};
				strcat(buffer, cat_str);
				strcat(buffer, argv[i]); 
				perror(buffer);
			}
			else
			{
            	while((k = read(r, buf, sizeof(buf))) > 0)
            	{
                	u = write(1, buf, k);
					if (u == -1)
					{
						char cat_str[] = "cat: ";
                		char buffer[1000] = {};
                		strcat(buffer, cat_str);
              		    strcat(buffer, argv[i]);
                		perror(buffer);
					}
            	}
				u = close(r);
				if (u == -1)
				{
					char cat_str[] = "cat: ";
                    char buffer[1000] = {};
                    strcat(buffer, cat_str);
                    strcat(buffer, argv[i]);
					perror(buffer);
				}
			}	
		}
	}
	return 0;
}
