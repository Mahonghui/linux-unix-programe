/*
	pwd1.c - simplified version of pwd

	start in current directory and recursively
	up way to root

	readdir() gets infos about files

	bug: print an empty string if run from '/'
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>


ino_t get_inode(char* );
void print_path(ino_t);
void i_to_name(ino_t, char*, int );

int main(int argc, char* argv[])
{
	if(argc == 1)
		print_path(get_inode(".")); // from current path
	else 
		if(argc == 2)
			print_path(get_inode(argv[1]));
		else
		{
			fprintf(stderr, "Usage: %s\n", argv[0]);
			exit(1);
		}

	putchar('\n');
	return 0;
}

void print_path(ino_t inode)
/*
	print path leading down to an object with inode
*/
{
	ino_t m_inode;
	char name[BUFSIZ];
	if(get_inode("..") != inode) // recursive up to root
	{
		chdir("..");			// gets to parent dir
		i_to_name(inode, name, BUFSIZ);	// find its name
		m_inode = get_inode(".");	// print head
		print_path(m_inode);		// recursively
		printf("/%s", name);		// backtrace printing
	}
}

void i_to_name(ino_t inode, char* namebuf, int bufsize)
{
	DIR* dir_ptr = NULL;
	struct dirent* direntp;
	dir_ptr = opendir(".");

	if(dir_ptr == NULL)
	{
		perror(".");
		exit(1);
	}

	// search dirctory for the filename specified by inode

	while((direntp = readdir(dir_ptr)) != NULL)
	{
		if(direntp->d_ino == inode)
		{
			strncpy(namebuf, direntp->d_name, bufsize);
			namebuf[bufsize-1] = '\0';
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "error looking for inum %d\n", inode);
	exit(1);
}

ino_t get_inode(char* direname)
/*
	return inode of dirname
*/
{
	struct stat info;
	if(stat(direname, &info) == -1)
	{
		fprintf(stderr, "Cannot stat");
		perror(direname);
		exit(1);
	}

	return info.st_ino;
}
