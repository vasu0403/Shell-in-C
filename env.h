void set_env(char *command)
{
	char *end_cmd;
	char *token = strtok_r(command, " ", &end_cmd);
	char *var = (char *)malloc(max_len * sizeof(char));
	char *val = (char *)malloc(max_len * sizeof(char));
	if(strlen(end_cmd)==0)
	{
		printf("Invalid number of arguments\n");
		return;
	}
	token = strtok_r(NULL, " ", &end_cmd);
	strcpy(var, token);
	if(strlen(end_cmd)==0)
		strcpy(val, "");
	else
	{
		token = strtok_r(NULL, " ", &end_cmd);
		strcpy(val, token);
	}
	if(strlen(end_cmd)!=0)
	{
		printf("Invalid number of arguments\n");
		return;
	}
	printf("%s %s\n", var, val);
}