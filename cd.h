void changeDir(char *args, int l, char *home_dir)
{
	char *dest = (char *)malloc(max_path * sizeof(char));
	char *temp = (char *)malloc(max_path * sizeof(char));
	int len = 0, i;
	while(l<strlen(args) && args[l]==' ')
		l++;
	if(l>=strlen(args))
	{
		if(chdir(home_dir)!=0)
			perror("Error");
		return;
	}
	for(; l<=strlen(args) && args[l] != ' '; l++)
		dest[len++] = args[l];
	dest[len] = '\0';
	if(dest[0] == '~')
	{
		strcpy(temp, home_dir);
		len = strlen(temp);
		for(i=1; i<strlen(dest); i++)
			temp[len++] = dest[i];
		strcpy(dest, temp);
	}
	if(chdir(dest)!=0)
		perror("Error");
	return;
}
