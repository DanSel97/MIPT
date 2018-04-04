#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
int main(int argc, char *argv[])
{
    struct stat sb;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    if (stat(argv[1], &sb) == -1) {
	perror("stat");
	exit(EXIT_FAILURE);
    }
    printf("  File: '%s'\n", argv[1]);
    printf("  Size: %lld", (long long) sb.st_size);
    printf("      	Blocks: %lld", (long long) sb.st_blocks);
    printf("         IO Block: %ld   ", (long) sb.st_blksize);
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
	printf("block device\n");
	break;
    case S_IFCHR:
	printf("character device\n");
	break;
    case S_IFDIR:
	printf("directory\n");
	break;
    case S_IFIFO:
	printf("FIFO/pipe\n");
	break;
    case S_IFLNK:
	printf("symlink\n");
	break;
    case S_IFREG:
	printf("regular file\n");
	break;
    case S_IFSOCK:
	printf("socket\n");
	break;
    default:
	printf("unknown?\n");
	break;
    }
    printf("Inode: %ld", (long) sb.st_ino);
    printf("     Links: %ld\n", (long) sb.st_nlink);
    printf("Access: (0%o/", sb.st_mode % 512);
    switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:
	printf("d");
	break;
    case S_IFLNK:
	printf("l");
	break;
    default:
	printf("-");
	break;
    }
    if (sb.st_mode & S_IRUSR) printf("r");
    else printf("-");
    if (sb.st_mode & S_IWUSR) printf("w");
    else printf("-");
    if (sb.st_mode & S_IXUSR) printf("x");
    else printf("-");
    if (sb.st_mode & S_IRGRP) printf("r");
    else printf("-");
    if (sb.st_mode & S_IWGRP) printf("w");
    else printf("-");
    if (sb.st_mode & S_IXGRP) printf("x");
    else printf("-");
    if (sb.st_mode & S_IROTH) printf("r");
    else printf("-");
    if (sb.st_mode & S_IWOTH) printf("w");
    else printf("-");
    if (sb.st_mode & S_IXOTH) printf("x");
    else printf("-");
    printf(")");
	struct passwd* pass = getpwuid(sb.st_uid);
    printf("  Uid: ( %ld/  %s)   Gid: ( %ld/  %s)\n", (long) sb.st_uid, pass->pw_name, (long) sb.st_gid, getgrgid(sb.st_gid)->gr_name);
    printf("Access: %s", ctime(&sb.st_atime));
    printf("Modify: %s", ctime(&sb.st_mtime));
    printf("Change: %s", ctime(&sb.st_ctime));
    exit(EXIT_SUCCESS);
}
