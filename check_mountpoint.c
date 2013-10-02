/*
 * check_mountpoint.c
 * nrpe check to see if mount point is mounted
 *
 * usage: check_mountpoint /tmp /blah /whatever
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define OK_STATE 0
#define WARNING_STATE 1
#define CRITICAL_STATE 2

int main(int argc, char *argv[])
{

        if(argc <= 1)
        {
                printf("Usage:\n%s <mount points>\n", argv[0]);
                exit(OK_STATE);
        }

	char buf[256];
	char dotup[] = "/..";
	int i;

	for ( i=1; i<argc; i++) {
		if(strncmp(argv[i], "/dev/", 5) == 0) {
			printf("ERROR: %s is a device.\n", argv[i]);
			exit(CRITICAL_STATE); 
		}

 
		snprintf(buf, sizeof buf, "%s%s", argv[i], dotup);
		struct stat mp, mp_parent;

		if (stat(argv[i], &mp) == -1) {
//			perror("stat");
			printf("CRITICAL: %s %s\n", argv[i],strerror(errno));
			exit(CRITICAL_STATE);
	        }

		stat(argv[i], &mp) ;
		stat(buf, &mp_parent);

		if(mp.st_dev == mp_parent.st_dev) {
			printf("CRITICAL: %s not mounted.\n", argv[i]);
			exit(CRITICAL_STATE);
		}
	}

	printf("OK: all mount points are mounted.\n");

}
