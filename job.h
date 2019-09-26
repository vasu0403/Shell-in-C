
void jobs(char *command)
{
	if(no_of_jobs == 0)
	{
		printf(BLU "No process currently running\n" RESET);
		return;
	}
	for(int i=0; i<no_of_jobs; i++)
	{
		char *status_path = (char *)malloc(max_len*sizeof(char));
		char *status = (char *)malloc(100 * sizeof(char));
		char *pid = (char *)malloc(100 * sizeof(char));
		sprintf(pid, "%d", all_jobs[i].job_pid);
		strcpy(status_path, "/proc/");
		strcat(status_path, pid);
		strcat(status_path, "/stat");
		FILE *f1 = fopen(status_path, "r");
		int c=0;
		while(fscanf(f1, "%s", status)!=EOF)
		{
			c++;
			if(c==3)
				break;
		}
		fclose(f1);
		if(strcmp(status, "T")==0)
			strcpy(all_jobs[i].job_status, "Stopped");
		else
			strcpy(all_jobs[i].job_status, "Running");
		printf(WHT "[%d] %s %s[%d]\n" RESET, all_jobs[i].job_number, all_jobs[i].job_status, all_jobs[i].job_name ,all_jobs[i].job_pid);
	}
	return;
}
void kjobs(char *command)
{
	char *end_cmd;
	char *token = strtok_r(command, " ", &end_cmd);
	if(strlen(end_cmd)==0)
	{
		printf("Invalid number of arguments\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	if(strlen(end_cmd)==0)
	{
		printf("Invalid number of arguments\n");
		return;
	}
	int job_num = atoi(token);
	token = strtok_r(NULL, " ", &end_cmd);
	if(strlen(end_cmd)!=0)
	{
		printf("Invalid number of arguments\n");
		return;
	}
	int sig = atoi(token);
	if(job_num > no_of_jobs)
	{
		printf("Invalid job number\n");
		return;
	}
	if(sig == 19)
		strcpy(all_jobs[job_num-1].job_status, "Stopped");
	kill(all_jobs[job_num-1].job_pid, sig);
	return;
}
void run_in_bg(char *command)
{
	char *end_cmd;
	char *token = strtok_r(command, " ", &end_cmd);
	token = strtok_r(NULL, " ", &end_cmd);
	int job_no = atoi(token);
	if(job_no > no_of_jobs)
	{
		printf("Invalid job number\n");
		return;
	}
	job_no--;
	pid_t pid_no = all_jobs[job_no].job_pid;
	kill(pid_no, SIGCONT);
	strcpy(all_jobs[job_no].job_status, "Running");
}
void run_in_fg(char *command)
{
	char *end_cmd;
	char *token = strtok_r(command," ", &end_cmd);
	if(strlen(end_cmd)==0)
	{
		printf("Too few arguments!\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	int job_no = atoi(token);
	int status;
	if(job_no > no_of_jobs)
	{
		printf("Invalid job number\n");
		return;
	}
	int pid = all_jobs[job_no -1].job_pid;
	printf("Running %s in foreground\n", all_jobs[job_no-1].job_name);
	remove_from_jobs(job_no-1);
	global_pid = pid;
	printf("%d\n", global_pid);
	int shell = getpid();
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(0, getpgid(pid));
	kill(pid, SIGCONT);
	signal(SIGTTOU, SIG_DFL);
	waitpid(pid, &status , WUNTRACED);
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(0, shell);
	signal(SIGTTOU, SIG_DFL);
	global_pid = shell_pid;

}
void overkill(char *command)
{
	char *end_cmd;
	char *token = strtok_r(command, " ", &end_cmd);
	if(strlen(end_cmd)!=0)
	{
		printf("Invalid argument!\n");
		return;
	}
	for(int i=0; i<no_of_jobs; i++)
		kill(all_jobs[i].job_pid, 9);
}