void execute_cron(char *command, char *home_dir, struct history* front, struct history* rear, int count)
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
void cronjob(char *command, char *home_dir, struct history* front, struct history* rear, int count)
{
	char temp[10000], exec_command[1000];
	strcpy(temp, command);
	char *end_cmd;
	char *token = strtok_r(command, " ", &end_cmd);
	if(strlen(end_cmd) == 0)
	{
		printf("Invalid argument\n");
		return;
	}	
	token = strtok_r(NULL, " ", &end_cmd);
	if(strcmp(token, "-c") != 0)
	{
		printf("Invalid argument\n");
		return;
	}
	if(strlen(end_cmd) == 0)
	{
		printf("Command to implement periodically missing\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	if(strcmp(token, "-t") == 0)
	{
		printf("Command to implement periodically missing\n");
		return;
	}
	char *args[1000];
	int len = 0;
	while(token != NULL)
	{
		if(strcmp(token, "-t") == 0)
			break;
		args[len++] = token;
		token = strtok_r(NULL, " ", &end_cmd);
	}
	if(token == NULL)
	{
		printf("Missing time period\n");
		return;
	}
	args[len] = NULL;
	strcpy(exec_command, args[0]);
	for(int i = 1; i<len; i++)
	{
		strcat(exec_command, " ");
		strcat(exec_command, args[i]);
	}
	if(strlen(end_cmd) == 0)
	{
		printf("Misiing time period\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	int t1 = atoi(token);
	if(strlen(end_cmd) == 0)
	{
		printf("Misiing arguments\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	if(strcmp(token, "-p") != 0)
	{
		printf("Invalid argument\n");
		return;
	}
	if(strlen(end_cmd) == 0)
	{
		printf("Mising time period\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	int t2 = atoi(token);
	if(strlen(end_cmd) !=0 )
	{
		printf("Invalud arguments\n");
		return;
	}
	int pid = fork();
	if(pid == 0)
	{
		for(int i = 0; i<t2; i+=t1)
		{
			sleep(t1);
			printf("\n");
			execute_cron(exec_command, home_dir, front, rear, count);
			print_prompt(home_dir);
		}
		exit(0);
	}
	else
	{
		strcpy(name[pid], temp);
		all_jobs[no_of_jobs].job_number = no_of_jobs + 1;
		all_jobs[no_of_jobs].job_pid = pid;
		strcpy(all_jobs[no_of_jobs].job_name, name[pid]);
		strcpy(all_jobs[no_of_jobs].job_status, "Running");
		job_pid_to_job_number[pid] = no_of_jobs;
		proc_type[pid] = 1;
		no_of_jobs++;
		return;
	}
}