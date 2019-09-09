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
#include "declarations.h"
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
#include "job.h"
void control()
{
	if(global_pid != shell_pid)
		kill(global_pid, SIGINT);
	global_pid = shell_pid;
}
void stop()
{
	;
}
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
	else if(strcmp(cmd, "quit")==0)
		exit(0);
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
	else if(strcmp(cmd, "jobs")==0)
		jobs(command);
	else if(strcmp(cmd, "kjob")==0)
		kjobs(command);
	else if(strcmp(cmd, "bg")==0)
		run_in_bg(command);
	else if(strcmp(cmd, "overkill")==0)
		overkill(command);
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
	shell_pid = getpid();
	global_pid = shell_pid;
	signal(SIGINT, control);
	signal(SIGTSTP, stop);
	clear();
	while(1)
	{ 
		
		print_prompt(home_dir);
		char *arg = NULL;
		size_t len = 0;
		check_for_background();
		getline(&arg, &len, stdin);
		check_for_background();
		arg[len-1] =  '\0';
		if(strcmp(arg, "\n")==0)continue;
		char *end_cmd;
		char *command = strtok_r(arg, ";", &end_cmd);
		while(command != NULL)
		{
			check_for_background();
			add_in_history(command, &front, &rear, &count);
			execute(command, home_dir, front, rear, count);
			check_for_background();
			command = strtok_r(NULL, ";", &end_cmd);
		}
		add_to_file(home_dir,front, rear);
	}

}
