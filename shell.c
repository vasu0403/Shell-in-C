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
void control()
{
	if(global_pid != shell_pid)
		kill(global_pid, SIGINT);
	global_pid = shell_pid;
}
void stop()
{
	// if(global_pid != shell_pid)
	// {
	// 	setpgid(global_pid, global_pid);
	// 	strcpy(name[global_pid], foreground_proc[global_pid]);
	// 	all_jobs[no_of_jobs].job_number = no_of_jobs + 1;
	// 	all_jobs[no_of_jobs].job_pid = global_pid;
	// 	strcpy(all_jobs[no_of_jobs].job_status, "Stopped");
	// 	strcpy(all_jobs[no_of_jobs].job_name, foreground_proc[global_pid]);
	// 	job_pid_to_job_number[global_pid] = no_of_jobs;
	// 	printf("\n[%d] + suspended  %s\n", no_of_jobs + 1, name[global_pid]);
	// 	no_of_jobs++;
	// 	kill(global_pid, SIGSTOP);
	// 	global_pid = shell_pid;
	// 	return;
	// }
}
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
#include "piping_redirection.h"
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
	else if(strcmp(cmd, "fg")==0)
		run_in_fg(command);
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
	int stdin_temp = dup(0);
	int stdout_temp = dup(1);
	clear();
	while(1)
	{ 
		
		print_prompt(home_dir);
		char *arg = NULL;
		size_t len = 0;
		int times = 0, j;
		check_for_background();
		// scanf(" %[^\n]s", arg);
		getline(&arg, &len, stdin);
		arg[strlen(arg)-1] = '\0';
		for(int i=0; i<strlen(arg); i++)
			key[i] = arg[i];
		for(int i=0; i<strlen(arg); i++)
		{
			if(key[i] == up[j])
			{
				j++;
				if(j==3)
				{
					times++;
					j = 0;
				}
			}
			else
				j = 0;
		}
		if(times)
		{
			struct history* temp;
			temp = front;
			if(times >= count)
				strcpy(arg, front->command);
			else
			{
				times = count - times;
				while(times > 0)
				{	
					temp = temp->next;
					times--;
				}
				strcpy(arg, temp->command);
			}
			print_prompt(home_dir);
			printf("%s\n", arg);
			fflush(stdout);
		}
		check_for_background();
		// arg[len-1] =  '\0';
		// printf("%s %d", arg, strlen(arg));
		if(strcmp(arg, "\n")==0)continue;
		char *end_cmd;
		char *command = strtok_r(arg, ";", &end_cmd);
		while(command != NULL)
		{
			int z1 = 1, z2 = 1;
			no_of_commands = 0;
			check_for_background();
			add_in_history(command, &front, &rear, &count);	
			if(empty_pipe(command))
			{
				printf("Invalid command\n");
				z2 = 0;
			}
			char *end_cmd2;
			char *command2 = strtok_r(command, "|", &end_cmd2);
			while(command2 != NULL)
			{
				strcpy(commands[no_of_commands], command2);
				no_of_commands++;
				command2 = strtok_r(NULL, "|", &end_cmd2);
			}
			if(bg_in_between())
			{
				printf("parse error near `|'\n");
				z1 = 0;
			}
			if(no_of_commands == 1 && z2)
			{	
				int check = 1;
				output_file_len= 0, input_file_len = 0;
				output = 0, input = 0;
				output = check_output_redirection(commands[0]);
				input = check_input_redirection(commands[0]);
				// printf("%d %d %s %d %s %d\n", output,input, output_file, output_file_len, input_file, input_file_len);
				if(output)
				{
					output_command_len = 0;
					find_command(commands[0]);
					strcpy(commands[0], output_command);
					commands[0][output_command_len] = '\0';
					if(output == 1)
					{
						int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
						dup2(fd, 1);
						close(fd);
					}
					else
					{
						int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
						dup2(fd, 1);
						close(fd);
					}
				}
				if(input)
				{
					output_command_len = 0;
					find_command(commands[0]);
					strcpy(commands[0], output_command);
					commands[0][output_command_len] = '\0';
					int fd = open(input_file, O_RDONLY);
					if(fd == -1)
					{
						check = 0;
						printf("Error: input file given does not exist\n");
					}
					else
					{	
						dup2(fd, 0);
						close(fd);
					}
				}
				if(check)
					execute(commands[0], home_dir, front, rear, count);
				dup2(stdin_temp, 0);
				dup2(stdout_temp, 1);
			}
			else if(z1 && z2)
			{	
				int in = dup(0);
				int out = dup(1);
				int check = 1;
				for(int i=0; i<no_of_commands; i++)
				{
					if(strcmp(commands[i], "quit") == 0)
							execute(commands[i], home_dir, front, rear, count);
					dup2(in, 0);
					close(in);
					output_file_len= 0;
					output = 0;
					input_file_len = 0;
					input = 0;
					input = check_input_redirection(commands[i]);
					if(input)
					{
						output_command_len = 0;
						find_command(commands[i]);
						strcpy(commands[i], output_command);
						commands[i][output_command_len] = '\0';
						int fd = open(input_file, O_RDONLY);
						if(fd == -1)
						{
							check = 0;
							printf("Error: input file given does not exist\n");
						}
						else
						{	
							dup2(fd, 0);
							close(fd);
						}
					}
					output = check_output_redirection(commands[i]);
					if(i==no_of_commands-1)
					{	
						out = dup(stdout_temp);
						if(output)
						{
							output_command_len = 0;
							find_command(commands[i]);
							strcpy(commands[i], output_command);
							commands[i][output_command_len] = '\0';
							if(output == 1)
							{
								int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
								out = dup(fd);
								close(fd);
							}
							else
							{
								int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
								out = dup(fd);
								close(fd);
							}
						}
					}
					else
					{
						int p[2];
						pipe(p);
						out = p[1];
						in = p[0];
					}
					dup2(out, 1);
					close(out);
					int pi = fork();
					if(pi == 0)
					{	
						if(check)
							execute(commands[i], home_dir, front, rear, count);
						exit(0);
					}
					else
					{
						int status;
						waitpid(pi, &status, 0);
						dup2(stdin_temp, 0);
						dup2(stdout_temp, 1);
					}
					dup2(stdin_temp, 0);
					dup2(stdout_temp, 1);					
				}
			}
			command = strtok_r(NULL, ";", &end_cmd);
			check_for_background();
		}
		add_to_file(home_dir,front, rear);
	}

}
