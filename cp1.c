/*
	cp1.c - uses read and write with buffer

	usage: cp1 src dst
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
#define COPYMODE 0644

void bail(char*, char*);

int main(int argc, char* argv[])
{
	int src_fd, dst_fd, n;
	char buffer[BUFFER_SIZE];

	if(argc != 3)
	{
		fprintf(stderr, "usage: %s source destination\n", *argv);
		exit(1);
	}

	if((src_fd = open(argv[1], O_RDONLY)) == -1)
		bail("Cannot open ", argv[1]);
	if((dst_fd = create(argv[2], COPYMODE)) == -1)
		bail("Cannot create", argv[2]);

	while((n = read(src_fd, buffer, BUFFER_SIZE)) > 0)
		if(write(dst_fd, buffer, n) != n)
			bail("Error write to ", argv[2]);

	if(n == -1)
		bail("Error read from ", argv[1]);

	if(close(src_fd) == -1 || close(dst_fd) == -1)
		bail("Error close files ", " ");
}

void bail(char* msg, char* which)
{
	fprintf(stderr, "Error: %s\n", msg);
	perror(which);
	exit(1);
}