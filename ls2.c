/*
	ls2.c - support ls -l 
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

void do_ls(char []);
void do_stat(char* );
void show_file_info(char* , struct stat*);
void mode_to_letters(int, char []);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);

int main(int argc, char* argv[])
{
	if(argc == 1)
		do_ls(".");
	else
		while(--argc)
		{
			printf("%s:\n", *++argv);
			do_ls(*argv);
		}
}

void do_ls(char dirname[])
{
	DIR* dir_ptr = NULL;
	struct dirent* dirent_ptr = NULL;

	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "cannot open %s:\n", dirname);

	else
	{
		while((dirent_ptr = readdir(dir_ptr)) != NULL)
			do_stat(dirent_ptr->d_name);
		closedir(dir_ptr);
	}
}

void do_stat(char* filename)
{
	struct stat info;

	if(stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

void show_file_info(char* filename, struct stat* info_ptr)
{
	char mode_str[11];

	mode_to_letters(info_ptr->st_mode, mode_str);

	printf("%s", mode_str);

	printf("%4d ", (int)info_ptr->st_nlink);
	printf("%-8s ", uid_to_name(info_ptr->st_uid));
	printf("%-8s ", gid_to_name(info_ptr->st_gid));
	printf("%8ld ", (long)info_ptr->st_size);
	printf("%.12s ", ctime(&info_ptr->st_mtimespec.tv_sec)+4);
	printf("%s\n", filename);
}


/*
	utility functions
*/
#include <string.h>
void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");
	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';

	if(mode&S_IRUSR) str[1] = 'r';
	if(mode&S_IWUSR) str[2] = 'w';
	if(mode&S_IXUSR) str[3] = 'x'; // owner's perms

	if(mode&S_IRGRP) str[4] = 'r';
	if(mode&S_IWGRP) str[5] = 'w';
	if(mode&S_IXGRP) str[6] = 'x'; // group's perms

	if(mode&S_IROTH) str[7] = 'r';
	if(mode&S_IWOTH) str[8] = 'w';
	if(mode&S_IXOTH) str[9] = 'x'; // other's perms
}

#include <pwd.h>

char* uid_to_name(uid_t uid)
{
	struct passwd* pw_ptr = NULL;
	static char numstr[10];

	if((pw_ptr = getpwuid(uid)) == NULL) // 没有用户名，返回UID
	{
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

#include <grp.h>

char* gid_to_name(gid_t gid)
{
	struct group* grp_ptr = NULL;
	static char numstr[10];
 
	if((grp_ptr = getgrgid(gid)) == NULL) // 没有组名，返回GID
	{
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

