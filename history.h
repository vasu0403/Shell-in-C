struct history {
	char command[10000];
	struct history* next;
};
void add_in_history(char* s, struct history** front, struct history** rear, int* count)
{
	struct history* temp = (struct history*)malloc(sizeof(struct history));
	strcpy(temp->command, s);
	temp->next = NULL;
	int len1, len2;
	if(*rear == NULL)
	{
		*front = temp;
		*rear = temp;
		(*count)++;
	}
	else
	{
		len1 = strlen(s);
		len2 = strlen((*rear)->command);
		if(s[len1-1]=='\n')
			s[len1-1]='\0';
		if(((*rear)->command)[len2-1]=='\n')
			((*rear)->command)[len2-1] = '\0';
		if(strcmp((*rear)->command, s)!=0)
		{
			(*rear)->next = temp;
			*rear = temp;
			(*count)++;
		}

	}
	if(*count > 20)
	{
		struct history* f;
		f = (*front);
		*front = (*front)->next;
		(*count)--;
		free(f);
	}
}
void read_file(char* home_dir, struct history** front, struct history** rear, int *count)
{
	FILE *f1 = fopen("store.txt", "r");
	char line[max_path], line2[max_path];
	int i=0;
	if(f1)
	{
		while (fgets(line, max_path, f1)!=NULL) 
		{
	      	// printf("**%ld %s\n", strlen(line), line);
	      	if(strlen(line) > 1)
	        	add_in_history(line, front, rear, count);
    	}
        fclose(f1);
	}
}
void add_to_file(char *home_dir, struct history* front, struct history* rear)
{
	char *temp = (char *)malloc(max_path * sizeof(char));
	strcpy(temp, home_dir);
	strcat(temp, "/store.txt");
	FILE *f;
	f = fopen(temp, "w");
	int i=0, len;
	char line[max_path], line2[max_path];
	while(front != NULL)
	{
		len = strlen(front->command);
		if((front->command)[len-1]!='\n')
			fprintf(f, "%s\n", front->command);
		else
			fprintf(f, "%s", front->command);
		front = front->next;
	}
	fclose(f);
	return;
}
void show_history(char *comm, int l, struct history* front, struct history* rear, int count)
{
	while(l<strlen(comm) && comm[l]==' ')
		l++;
	if(l>=strlen(comm))
	{
		if(count>10)
		{
			while(count!=10)
			{
				count--;
				front = front->next;
			}
		}
		while(front!=NULL)
		{
	      	if(strlen(front->command) > 1)
			{
				int len = strlen(front->command);
				if((front->command)[len-1]!='\n')
					printf("%s\n", front->command);
				else
					printf("%s", front->command);
			}
			front = front->next;
		}
	}
	else
	{
		char a[2];
		int i=0, num;
		while(l<strlen(comm) && i<2)
			a[i++] = comm[l++];
		num = atoi(a);
		if(num > count)
		{
			printf("Error: This many commands in history does not exist\n");
			return;
		}
		while(count!=num)
		{
			count--;
			front = front->next;
		}
		while(front!=NULL)
		{
			if(strlen(front->command) > 1)
			{
				int len = strlen(front->command);
				if((front->command)[len-1]!='\n')
					printf("%s\n", front->command);
				else
					printf("%s", front->command);
			}
			front = front->next;
		}
	}
}