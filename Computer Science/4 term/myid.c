#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <string.h>
int main(int argc, char *argv[])
{
    	FILE* file;
	file = fopen("/etc/group", "r");
	if (argc == 1)
	{
		uid_t uid = getuid();
		struct passwd* passw = getpwuid(uid);
		int gid = (int)passw->pw_gid;
		printf("uid=%d(%s) ", (int)passw->pw_uid, passw->pw_name);
        printf("gid=%d(%s) ", (int)passw->pw_gid, passw->pw_name);
        printf("groups=%d(%s)", (int)getgrgid(gid)->gr_gid, passw->pw_name);
        struct group* mygroup = getgrent();
        for (; mygroup != NULL; mygroup = getgrent())
            {
        int i = 0;
        int t = 0;
        while ((mygroup->gr_mem)[i] != NULL)
            {
            if (strcmp((mygroup->gr_mem)[i], passw->pw_name) == 0)
            {
                t = 1;
            };
            i++;
            }
        if (t == 1)
        {
            printf(",%d(%s)", mygroup->gr_gid, mygroup->gr_name);
        }
        }
        printf("\n");		
	}
	else if (argc == 2)
	{
	if (getpwnam(argv[1]) == NULL)
	{
		printf("id: %s: no such user\n", argv[1]);
	}
	else
	{
            printf("uid=%d(%s) ", (int)getpwnam(argv[1])->pw_uid, argv[1]);
            printf("gid=%d(%s) ", (int)getpwnam(argv[1])->pw_gid, argv[1]);
            printf("groups=%d(%s)", (int)getgrnam(argv[1])->gr_gid, argv[1]);
	    struct group* mygroup = getgrent();
 	    for (; mygroup != NULL; mygroup = getgrent())
            {
		int i = 0;
		int t = 0;
        while ((mygroup->gr_mem)[i] != NULL)
	    	{
			if (strcmp((mygroup->gr_mem)[i], argv[1]) == 0)
			{
				t = 1;
			};
			i++;
  	    	}
		if (t == 1)
		{
			printf(",%d(%s)", mygroup->gr_gid, mygroup->gr_name);
		}
	    }
	    printf("\n");
	}
	}
	else
	{
		fprintf(stdout, "id: extra operand '%s'\n'", argv[2]);
	}
	return 0;
}

