void jobs(char *command)
{
	for(int i=0; i<no_of_jobs; i++)
	{
		printf("[%d] %s %s[%d]\n", all_jobs[i].job_number, all_jobs[i].job_status, all_jobs[i].job_name ,all_jobs[i].job_pid);
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
	job_no--;
	if(job_no > no_of_jobs)
	{
		printf("Invalid job number\n");
		return;
	}
	pid_t pid_no = all_jobs[job_no].job_pid;
	printf("%d\n", pid_no);
	kill(pid_no, SIGCONT);
	strcpy(all_jobs[job_no].job_status, "Running");
}
