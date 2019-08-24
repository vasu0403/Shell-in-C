bool keyPress()
{
	struct termios oldt, newt;
	int bytes;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ioctl(STDIN_FILENO, FIONREAD, &bytes);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return bytes >0;
}
void interrupt(int n)
{
	char c;
	int count = 0;
	FILE *f1 = fopen("/proc/interrupts", "r");
	char *inter = (char *)malloc(max_len * sizeof(char));
	fgets(inter, max_len, f1);
	char *end_cmd, *end_cmd1;
	char *token1 = strtok_r(inter, " ", &end_cmd);
	char *token2;
	while(token1!=NULL)
	{
		printf("%s\t\t", token1);
		token1 = strtok_r(NULL, " ", &end_cmd);
		count++;
	}
	count--;
	printf("\n");
	fclose(f1);
	while(1)
	{
		if(keyPress())
		{
			c = getchar();
			if(c=='q')
			{	
				printf("\n");
				fflush(stdout);
				return;
			}
		}
		int count2 = 0;
		FILE *f1 = fopen("/proc/interrupts", "r");
		fgets(inter, max_len, f1);
		fgets(inter, max_len, f1);
		fgets(inter, max_len, f1);
		token2 = strtok_r(inter, " ", &end_cmd1);
		token2 = strtok_r(NULL, " ", &end_cmd1);
		while(token2!=NULL)
		{
			if(keyPress())
			{
				c = getchar();
				if(c=='q')
				{	
					printf("\n");
					fflush(stdout);
					return;
				}
			}
			printf("%s\t\t", token2);
			token2 = strtok_r(NULL, " ", &end_cmd1);
			count2++;
			if(count2 == count)
				break;
		}
		printf("\n");
		if(keyPress())
		{
			c = getchar();
			if(c=='q')
			{	
				printf("\n");
				return;
			}
		}
		sleep(n);
	}	
}
void dirty(int n)
{
	char c;
	while(1)
	{	
		if(keyPress())
		{
			c = getchar();
			if(c=='q')
			{	
				printf("\n");
				return;
			}
		}
		FILE *f1 = fopen("/proc/meminfo", "r");
		char *dir = (char *)malloc(max_len*sizeof(char));
		while(fgets(dir, max_len, f1)!=NULL)
		{
			char *end_cmd;
			char *token1 = strtok_r(dir, ":", &end_cmd);
			if(keyPress())
			{
				c = getchar();
				if(c=='q')
				{	
					printf("\n");
					return;
				}
			}
			if(strcmp(token1, "Dirty")==0)
			{
				token1 = strtok_r(NULL, ":", &end_cmd);
				int l = 0;
				while(token1[l]==' ')
					l++;
				while(l<strlen(token1))
					printf("%c", token1[l++]);
			}
		}
		if(keyPress())
		{
			c = getchar();
			if(c=='q')
			{	
				printf("\n");
				return;
			}
		}
		fclose(f1);
		sleep(n);
	}
}
void watch(char* command)
{
	char* end_cmd;
	char *token1 = strtok_r(command, " ", &end_cmd);
	token1 = strtok_r(NULL, " ", &end_cmd);
	if(strcmp(token1, "-n")!=0)
	{
		printf("Invalid flag\n");
		return;
	}
	if(strlen(end_cmd)==0)
	{
		printf("Arguments missing\n");
		return;
	}
	token1 = strtok_r(NULL, " ", &end_cmd);
	int time = atoi(token1);
	if(time == 0)
	{
		printf("Invalid argument\n");
		return;
	}
	if(strlen(end_cmd)==0)
	{
		printf("Arguments missing\n");
		return;
	}
	token1 = strtok_r(NULL, " ", &end_cmd);
	if(strlen(end_cmd)!=0)
	{
		printf("Invalid argument\n");
		return;
	}
	if(strcmp(token1, "interrupt")==0)
		interrupt(time);
	else if(strcmp(token1, "dirty")==0)
		dirty(time);
	else
		printf("Invalid argument\n");
	return;
}