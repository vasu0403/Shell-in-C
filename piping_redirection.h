int bg_in_between()
{
	for(int i = 0; i < no_of_commands - 1 ; i++)
	{
		for(int j=0; j<strlen(commands[i]); j++)
			if(commands[i][j] == '&')
				return 1;
	}
	return 0;
}
int empty_pipe(char *cmd)
{
	int ret = 0;
	for(int i=0; i<strlen(cmd); i++)
	{
		if(cmd[i] == '|')
			ret = 1;
		if(cmd[i] != '|' && cmd[i] != ' ')
			ret = 0;
	}
	return ret;
}
int check_output_redirection(char *cmd)
{
	int i, flag = 1;
	for(i=0; i<strlen(cmd); i++)
		if(cmd[i] == '>')
			break;
	if(i == strlen(cmd))
		return 0;
	if(i == strlen(cmd) -1)
		return 1;
	i++;
	if(cmd[i] == '>')
	{	
		flag = 2;
		i++;
	}
	while(i < strlen(cmd) && cmd[i] == ' ')
		i++;
	while(i < strlen(cmd) && cmd[i] != ' ')
		output_file[output_file_len++] = cmd[i++];
	output_file[output_file_len] = '\0';
	return flag;
}
int check_input_redirection(char *cmd)
{
	int i;
	for(i=0; i<strlen(cmd); i++)
		if(cmd[i] == '<')
			break;
	if(i == strlen(cmd))
		return 0;
	i++;
	while(i < strlen(cmd) && cmd[i] == ' ')
		i++;
	while(i < strlen(cmd) && cmd[i] != ' ' && cmd[i] != '>')
		input_file[input_file_len++] = cmd[i++];
	input_file[input_file_len] = '\0';
	return 1;
}
void find_command(char *cmd)
{
	for(int i=0; i<strlen(cmd) && cmd[i]!='>' && cmd[i] != '<'; i++)
		output_command[output_command_len++] = cmd[i];
	return;
}