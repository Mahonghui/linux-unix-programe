/*
	who1.c - first version of who programe
	open, read UTMP file, and display results
*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST

void show_info(struct utmp*);

int main(int argc, char const *argv[])
{
	struct utmp record;
	int utmpfd;
	int qty = sizeof(record);

	if((utmpfd = open(_PATH_UTMP, O_RDONLY)) == -1)	// _PATH_UTMP is in utmp.h
		{
			perror(_PATH_UTMP);
			exit(1);
		}

	while(read(utmpfd, &record, qty) == qty)
		show_info(&record);

	close(utmpfd);
	return 0;
}

/*
	show_info() - display contents of utmp struct formattedly.
*/

void show_info(struct utmp* utp)
{
	printf("% -8.8s", utp->ut_name);	// logname
	printf(" ");
	printf("% -8.8s", utp->ut_line);	// the tty
	printf(" ");
	printf("% -10ld", utp->ut_time);	// log time
	printf(" ");
	#ifdef SHOWHOST
	printf("(%s)", utp->ut_host);
	#endif
	printf("\n");
}