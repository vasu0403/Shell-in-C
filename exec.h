void remove_from_jobs(int x)
{
	for(int i=x; i<no_of_jobs-1; i++)
	{
		all_jobs[i].job_number = i+1;
		strcpy(all_jobs[i].job_name, all_jobs[i+1].job_name);
		strcpy(all_jobs[i].job_status, all_jobs[i+1].job_status);
		all_jobs[i].job_pid = all_jobs[i+1].job_pid;
		job_pid_to_job_number[all_jobs[i+1].job_pid] = i;
	}
	no_of_jobs--;
	return;
}
void check_for_background()
{
	int pid;
	int status;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		char* exit = (char *)malloc(max_len*sizeof(char));
		char* exit_status = (char *)malloc(max_len*sizeof(char));
		sprintf(exit, RED "%s with pid %d exited.\n" RESET, name[pid], pid);
	    int ret = WEXITSTATUS(status);
	    if(ret==0)         
	    	sprintf(exit_status, RED "normally\n" RESET);
	    else
	    	sprintf(exit_status, RED "abnormally\n" RESET);
		// printf("%d %d %d %d %d\n", global_pid, pid, shell_pid, getpid(), getppid());
	    if(pid >0 && pid != global_pid)	
	    {
			remove_from_jobs(job_pid_to_job_number[pid]);
		    write(2, exit, strlen(exit));
		    write(2, exit_status, strlen(exit_status));
		}
		free(exit);
	}
}
// void handler(int sig)
// {
// 	pid_t pid;
// 	int status;
// 	char* exit = (char *)malloc(max_len*sizeof(char));
// 	char* exit_status = (char *)malloc(max_len*sizeof(char));
// 	pid = waitpid(0, &status, WNOHANG);
// 	sprintf(exit, "\n%s with pid %d exited.\n", name[pid], pid);
//     int ret = WEXITSTATUS(status);
//     if(ret==0)         
//     	sprintf(exit_status, "normally\n");
//     else
//     	sprintf(exit_status, "abnormally\n");
//     if(pid >0)
//     {
// 		remove_from_jobs(job_pid_to_job_number[pid]);
// 	    write(2, exit, strlen(exit));
// 	    write(2, exit_status, strlen(exit_status));
// 	    print_prompt(home_dir);
// 	}
// 	free(exit);
// 	return;
// }
void foreground(char *cmd)
{
	char foreground_name[30];
	strcpy(foreground_name, cmd);
	char *end_cmd;
	char *token=strtok_r(cmd," ",&end_cmd);
	char *args[1000];
	int i=0;
	while(token!=NULL)
	{
		args[i++]=token;
		token = strtok_r(NULL, " ", &end_cmd);
	}
	args[i] = NULL;
	int pid = fork();
	int status;
	if(pid == 0)
	{
		setpgid(0, 0);
		if(execvp(args[0], args)!=0)
			printf("Error: No such command\n");
		exit(0);
	}
	else if(pid<0)
	{	
		perror("Error");
	}
	else
	{	
		int shell_pid = getpid();
		strcpy(foreground_proc[pid], foreground_name);
		proc_type[pid] = 0;
		global_pid = pid;
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		tcsetpgrp(0, pid);
		tcsetpgrp(1, pid);
		waitpid(pid, &status, WUNTRACED);
		tcsetpgrp(0, getpgid(shell_pid));
		tcsetpgrp(1, getpgid(shell_pid));
		signal(SIGTTOU, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		if(WIFSTOPPED(status))
		{
			// setpgid(global_pid, global_pid);
			strcpy(name[global_pid], foreground_proc[global_pid]);
			all_jobs[no_of_jobs].job_number = no_of_jobs + 1;
			all_jobs[no_of_jobs].job_pid = global_pid;
			strcpy(all_jobs[no_of_jobs].job_status, "Stopped");
			strcpy(all_jobs[no_of_jobs].job_name, foreground_proc[global_pid]);
			job_pid_to_job_number[global_pid] = no_of_jobs;
			printf("\n[%d] + suspended  %s\n", no_of_jobs + 1, name[global_pid]);
			no_of_jobs++;
			kill(global_pid, SIGSTOP);
			global_pid = shell_pid;
		}
		global_pid = shell_pid;
	}
}
void background(char *cmd)
{
	// printf("%s\n", cmd);
	char *args[1000];
	char *end_cmd;
	char *token=strtok_r(cmd," ",&end_cmd);
	char *temp = (char *)malloc(max_len * sizeof(char));
	int i=0;
	while(token!=NULL)
	{
		if(strcmp(token, "&")!=0)
		{	
			args[i++]=token;
			strcat(temp, token);
			strcat(temp, " ");
		}
		token = strtok_r(NULL, " ", &end_cmd);
	}
	args[i] = NULL;
	int pid = fork();
	if(pid == 0)
	{
		setpgid(0, 0);
		if(execvp(args[0], args)!=0)
			printf("Error: No such command\n");
		exit(0);
	}
	else if(pid < 0)
	{	
		perror("Error");
	}
	else
	{
		setpgid(pid,pid);
		strcpy(name[pid], args[0]);
		all_jobs[no_of_jobs].job_number = no_of_jobs + 1;
		all_jobs[no_of_jobs].job_pid = pid;
		strcpy(all_jobs[no_of_jobs].job_status, "Running");
		strcpy(all_jobs[no_of_jobs].job_name, temp);
		job_pid_to_job_number[pid] = no_of_jobs;
		proc_type[pid] = 1;
		no_of_jobs++;
	}
	return;
}
void exec(char *cmd, char *command, int l)
{
	for(int i=0; i<strlen(command); i++)
	{
		if(i!=0 && command[i]=='&' && command[i-1]==' ' && (command[i+1]==' ' || command[i+1] == '\0')  )
		{	
			background(command);
			return;
		}
	}
	foreground(command);
	return;
}