void showDir(char *command, int l, char *home_dir)
{
	char *cur_dir = (char *)malloc(max_path*sizeof(char));
	for(; l<strlen(command); l++)
	{
		if(command[l]!=' ')
		{
			printf("pwd: too many arguments\n");
			return;
		}
	}
	getcwd(cur_dir, max_path);
	replace(cur_dir, strlen(home_dir), home_dir);
	printf("%s\n", cur_dir);
}