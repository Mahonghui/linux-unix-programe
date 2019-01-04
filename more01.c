/*
	more01.c - version 0.1 of more
	read and print 24 lines then pause for a few special commands
*/

#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE*);
int see_more();

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
	char line[PAGELEN];
	int num_of_lines = 0;
	int see_more(), reply;
	while(fgets(line, PAGELEN, fp)){ // more input
		if(num_of_lines == PAGELEN) // full screen?
		{
			reply = see_more(); // yes, ask user
			if(reply == 0)	// no, done
				break;
			num_of_lines -= reply; // reset count
		}

		if(fputs(line, stdout) == EOF) // show line
			exit(1);

		num_of_lines++;
	}
}

int see_more()
/*
	print message, wait for response, return # of lines to advance
	q means no, space means yes, CR means one line
*/
{
	int c;
	printf("\033[7m more? \033[m");
	while((c = getchar()) != EOF)
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
