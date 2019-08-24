char name[100000][20];
void handler(int sig)
{
	pid_t pid;
	int status;
	char* exit = (char *)malloc(max_len*sizeof(char));
	char* exit_status = (char *)malloc(max_len*sizeof(char));
	pid = waitpid(0, &status, WNOHANG);
	sprintf(exit, "\n%s with pid %d exited.\n", name[pid], pid);
	if ( WIFEXITED(status) ) 
    { 
        int ret = WEXITSTATUS(status);
        if(ret==0)         
        	sprintf(exit_status, "normally\n");
        else
        	sprintf(exit_status, "abnormally\n");
    } 	
   if(pid >0)
   {
	    write(2, exit, strlen(exit));
	    write(2, exit_status, strlen(exit_status));
	    print_prompt(home_dir);
	}
	free(exit);
	return;
}
void foreground(char *cmd)
{
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
		if(execvp(args[0], args)!=0)
			printf("Error: No such command\n");
		exit(0);
	}
	else if(pid<0)
	{	
		perror("Error");
	}
	else
		waitpid(pid, &status, 0);
}
void background(char *cmd)
{
	char *args[1000];
	char *end_cmd;
	char *token=strtok_r(cmd," ",&end_cmd);
	int i=0;
	while(token!=NULL)
	{
		if(strcmp(token, "&")!=0)
			args[i++]=token;
		token = strtok_r(NULL, " ", &end_cmd);
	}
	args[i] = NULL;
	int pid = fork();
	signal(SIGCHLD, handler);
	if(pid == 0)
	{
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
			strcpy(name[pid], args[0]);
			// printf("%s\n", name[pid]);
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