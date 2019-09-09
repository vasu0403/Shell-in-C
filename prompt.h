void get_to_curr_dir(const char *path, char* home_dir)
{
	char *init = (char *)malloc(max_path*sizeof(char));
	getcwd(init, max_path);
	char *apath = (char *)malloc(max_path*sizeof(char));
	int r, l;
	for(r = strlen(path)-1; r>=0; r--)
		if(path[r]=='/')
			break;
	for(l=0; l<=r; l++)
	{
		apath[l] = path[l];
	}
	chdir(apath);
	getcwd(home_dir, max_path);
	chdir(init);
}
void replace(char *cur_dir, unsigned int l, char *home_dir)
{
	char *temp = (char *)malloc(max_path*sizeof(char));
	int i;
	temp[0] = '~';
	if(strlen(cur_dir) < strlen(home_dir))
		return;
	for(i=0; i<l; i++)
		if(cur_dir[i]!=home_dir[i])
			return;
	if(cur_dir[i]!='\0' && cur_dir[i]!='/')
		return;
	// printf("123\n");
	for(i=l; i<strlen(cur_dir); i++)
	{	
		temp[i-l+1] = cur_dir[i];
	}
	temp[i-l+1] = '\0';
	strcpy(cur_dir, temp);
}
void print_prompt(char *home_dir)
{
	struct utsname uts;
	char user_name[100];
	char* curr_dir = (char *)malloc(max_path*sizeof(char));
	if(uname(&uts)==0)
	{
		if(getlogin_r(user_name, 100)==0)
		{
			if(getcwd(curr_dir, max_path)!=NULL)
			{
				replace(curr_dir, strlen(home_dir), home_dir);
				printf( CYN "<%s@%s:" YEL "%s" CYN ">" RESET, user_name, uts.nodename, curr_dir);
				fflush(stdout);
			}
			else
				perror("Error");
		}
		else
			perror("Error");
	}
	else
		perror("Error:");
}