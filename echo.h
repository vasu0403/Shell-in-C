void echo(char *command, int l)
{
	char *print = (char *)malloc(max_len * sizeof(char));
	int i=0, c=0;
	while(l<strlen(command) && command[l]==' ')
		l++;
	for(; l<strlen(command); l++)
	{
		if(command[l]=='\"')
			c++;
		else
			print[i++] = command[l];
	}
	if(c&1)
	{
		printf("Error: Missing end quote\n");
		return;
	}
	printf("%s\n", print);
	return;
}