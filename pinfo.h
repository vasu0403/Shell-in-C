void pinfo(char* command, int l)
{
	char *pid = (char *)malloc(max_len*sizeof(char));
	char *vm_path = (char *)malloc(max_len*sizeof(char));
	char *status_path = (char *)malloc(max_len*sizeof(char));
	char *exe_path = (char *)malloc(max_len*sizeof(char));
	char *vm = (char *)malloc(100*sizeof(char));
	char *status = (char *)malloc(100*sizeof(char));
	char *exe = (char *)malloc(max_path*sizeof(char));
	int len = 0, c=0; 
	while(l<strlen(command) && command[l]==' ')
		l++;
	if(l>=strlen(command))
	{
		int t = getppid();
		sprintf(pid, "%d", t);
	}
	else
	{
		while(l<strlen(command))
		{
			pid[len++] = command[l++];
		}
	}
	char *path = (char *)malloc(max_len*sizeof(char));
	strcpy(path, "/proc/");
	strcat(path, pid);
	strcpy(vm_path, path);
	strcpy(status_path, path);
	strcpy(exe_path, path);
	strcat(vm_path, "/statm");
	strcat(status_path, "/stat");
	strcat(exe_path, "/exe");
	readlink(exe_path, exe, max_path);
	FILE *f1 = fopen(status_path, "r");
	if(f1 == NULL)
	{
		printf("Invalid pid\n");
		return;
	}
	FILE *f2 = fopen(vm_path, "r");
	while(fscanf(f1, "%s", status)!=EOF)
	{
		c++;
		if(c==3)
			break;
	}
	fscanf(f2, "%s", vm);
	printf("pid -- %s\n", pid);
	printf("Process Status -- %s\n", status);
	printf("memory -- %s\n", vm);
	printf("Exutable Path -- %s\n", exe);
	fclose(f2);
	fclose(f1);
}