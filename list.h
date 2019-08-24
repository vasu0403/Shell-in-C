void permissions(char *file, char *print)
{
	int permission[9], len = 1, i, l1=0;
	char *user = (char *)malloc(100*sizeof(char));
	char *group = (char *)malloc(100*sizeof(char));
	char *time = (char *)malloc(100*sizeof(char));
	char *month = (char *)malloc(100*sizeof(char));
	char *day = (char *)malloc(100*sizeof(char));
	char *ti = (char *)malloc(100*sizeof(char));
	struct stat filestat;
	permission[0] = S_IRUSR;
	permission[1] = S_IWUSR;
	permission[2] = S_IXUSR;
	permission[3] = S_IRGRP;
	permission[4] = S_IWGRP;
	permission[5] = S_IXGRP;
	permission[6] = S_IROTH;
	permission[7] = S_IWOTH;
	permission[8] = S_IXOTH;
	char p[] = "rwx";
	char *temp = (char *)malloc(10*sizeof(char));
	stat(file, &filestat);
	strcpy(time, ctime(&filestat.st_mtime));
	i=0, l1=0;
	while(time[i]!=' ')
		i++;
	i++;
	while(time[i]!=' ')
		month[l1++] = time[i++];
	month[3] = '\0';
	i++;
	l1=0;
	while(l1<2)
	{	
		day[l1++] = time[i++];
	}
	day[2] = '\0';
	i++;
	l1=0;
	while(l1<5)
		ti[l1++] = time[i++];
	ti[5] = '\0';	
	if(S_ISDIR(filestat.st_mode))
		temp[0] = 'd';
	else if(S_ISBLK(filestat.st_mode))
		temp[0] = 'b';
	else if(S_ISCHR(filestat.st_mode))
		temp[0] = 'c';
	else if(S_ISLNK(filestat.st_mode))
		temp[0] = 'l';
	else if(S_ISSOCK(filestat.st_mode))
		temp[0] = 'b';
	else
		temp[0] = '-';
	for(i=0; i<8; i++)
	{
		if(permission[i]&filestat.st_mode)
			temp[len++] = p[i%3];
		else
			temp[len++] = '-';
	}
	if(filestat.st_mode & S_IXUSR)
		temp[len++] = 'x';
	else
		temp[len++] = '-';
	temp[10] = '\0';
	ti[5] = '\0';
	struct passwd *user_id= getpwuid(filestat.st_uid);
	struct group *group_id= getgrgid(filestat.st_gid);
	int t;
	t = filestat.st_mtime;
	// printf("%s\n",time);
	printf("%s\t%ld\t%s\t%s\t%ld\t%s %s %s %s\n", temp, filestat.st_nlink, user_id->pw_name, group_id->gr_name, filestat.st_size, month, day, ti, file);
	free(temp);
	free(user);
	free(group);
	free(time);
	free(month);
	free(day);
	free(ti);
}
void list(char *dir, int f)
{
	DIR *dr;
	struct dirent* file;
	dr = opendir(dir);
	if(dr == NULL)
	{
		printf("Error: Invalid directory\n");
		return;
	}
	char *filename = (char *)malloc(max_len*sizeof(char));
	while((file = readdir(dr))!=NULL)
	{	
		strcpy(filename, file->d_name);
		if(filename[0]!='.' || f)
			printf("%s\n", filename);
	}
	closedir(dr);
	return;
}
void listLong(char *dir, int f)
{
	DIR *dr;
	struct dirent* file;
	dr = opendir(dir);
	if(dr == NULL)
	{
		printf("Error: Invalid directory\n");
		return;
	}
	char *filename = (char *)malloc(max_len*sizeof(char));
	char *print = (char *)malloc(max_len*sizeof(char));
	while((file = readdir(dr))!=NULL)
	{
		strcpy(filename, file->d_name);
		if(filename[0]!='.' || f)
		{
			permissions(filename, print);
		}
	}
}
void ls(char *command, int l, char *home_dir)
{
	int flag = 0, i, len = 0, f1=0, f2=0, len2;
	char *dir = (char *)malloc(max_len*sizeof(char));
	char *temp = (char *)malloc(max_len*sizeof(char));
	for(i=l; i<strlen(command); i++)
	{
		if(command[i] == '-')
			flag = 1;
		else if(command[i]!=' ')
			dir[len++] = command[i];
	}
	if(flag)
	{	
		for(i=0; i<strlen(dir); i++)
		{
			if(dir[i]=='a')
				f1 = 1;
			if(dir[i]=='l')
				f2 = 1;
		}
		strcpy(dir, ".");
	}
	else if(strlen(dir)==0)
		strcpy(dir, ".");
	if(dir[0]=='~')
	{
		strcpy(temp, home_dir);
		len2 = strlen(temp);
		for(i=1; i<len; i++)
			temp[len2++] = dir[i];
		temp[len2] = '\0';
		strcpy(dir, temp);
		printf("%s\n", dir);
	}
	if(f2)
	{	
		listLong(dir, f1);
	}
	else
		list(dir, f1);
}