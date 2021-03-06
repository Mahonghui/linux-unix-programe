/*
	more02.c - version 0.2 of more
	read and print 24 lines then pause for a few special commands
	feature of 0.2: read from /dev/tty for commands
*/

#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE*);
int see_more(FILE*);

int main(int argc, char* argv[])
{
	FILE* fp = NULL;
	if(argc == 1)
		do_more(stdin);
	else
		while(--argc)
			if((fp = fopen(* ++argv, "r")) != NULL)
			{
				do_more(fp);
				fclose(fp);
			}
			else
				exit(1);
	return 0;
}

void do_more(FILE* fp)
	/*
	read PAGELINE lines, then call see_more() for further intructions
	*/
{
	char line[LINELEN];
	int num_of_lines = 0;
	int  reply;

	FILE* fp_tty = fopen("/dev/tty", "r");	// NEW: cmd stream
	if(fp_tty == NULL)
		exit(1);

	while(fgets(line, PAGELEN, fp)){ // more input
		if(num_of_lines == PAGELEN) // full screen?
		{
			reply = see_more(fp_tty); // NEW: pass FILE
			if(reply == 0)	// no, done
				break;
			num_of_lines -= reply; // reset count
		}

		if(fputs(line, stdout) == EOF) // show line
			exit(1);

		num_of_lines++;
	}
	fclose(fp_tty);
}

int see_more(FILE* cmd)	//NEW: accept stream 
/*
	print message, wait for response, return # of lines to advance
	q means no, space means yes, CR means one line
*/
{
	int c;
	printf("\033[7m more? \033[m");
	while((c = getc(cmd)) != EOF)	//NEW: reads from tty
	{
		if(c == 'q')
			return 0; 
		if(c == ' ')	// show next page 
			return PAGELEN;
		if(c == '\n') // unix-like OS
			return 1;	//show one line
	}
	return 0;
}
