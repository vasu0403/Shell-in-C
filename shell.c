#include<sys/utsname.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<limits.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<wait.h>
#include<termios.h>
#include<stdbool.h>
#include<sys/ioctl.h>
const int max_len = 1e5, max_path = 1e4;
char home_dir[100000];
#include "prompt.h"
#include "cd.h"
#include "pwd.h"
#include "echo.h"
#include "list.h"
#include "pinfo.h"
#include "history.h"
#include "exec.h"
#include "watch.h"
#include "env.h"
void execute(char *command, char *home_dir, struct history* front, struct history* rear, int count)
{
	int i = 0, len=0;
	while(command[i]==' ')
		i++;
	char* cmd = (char*)malloc(max_len*sizeof(char));
	while(i<strlen(command) && command[i]!=' ')
		cmd[len++] = command[i++];
	cmd[len] = '\0';
	if(strcmp(cmd, "cd") == 0)
		changeDir(command, i+1, home_dir);
	else if(strcmp(cmd, "pwd") == 0)
		showDir(command, i+1, home_dir);
	else if(strcmp(cmd, "echo") == 0)
		echo(command, i+1);
	else if(strcmp(cmd, "ls") == 0)
		ls(command, i+1, home_dir);
	else if(strcmp(cmd, "pinfo") == 0)
		pinfo(command, i+1);
	else if(strcmp(cmd, "history") == 0)
		show_history(command, i+1, front, rear, count);
	else if(strcmp(cmd, "nightswatch") == 0)
		watch(command);
	else if(strcmp(cmd, "setenv") == 0)
		set_env(command);
	else if(strcmp(cmd, "unsetenv") == 0)
		unset_env(command);
	else
		exec(cmd, command, i+1);

}
int main(int argc, char const *argv[])
{	
	char *temp = (char *)malloc(max_path*sizeof(char));
	struct history *front, *rear;
	front = NULL;
	rear = NULL;
	setlinebuf(stdout);
	int count = 0;
	get_to_curr_dir(argv[0], home_dir);
	read_file(home_dir, &front, &rear, &count);
	while(1)
	{ 
		print_prompt(home_dir);
		char *arg = (char *)malloc( max_len * sizeof(char));
		scanf(" %[^\n]s", arg);
		char *command = strtok(arg, ";");
		while(command != NULL)
		{
			add_in_history(command, &front, &rear, &count);
			execute(command, home_dir, front, rear, count);
			command = strtok(NULL, ";");
		}
		add_to_file(home_dir,front, rear);
	}

}
