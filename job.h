void jobs(char *command)
{
	for(int i=0; i<no_of_jobs; i++)
	{
		printf("[%d] %s %s [%d]\n", all_jobs[i].job_number, all_jobs[i].job_name, all_jobs[i].job_status ,all_jobs[i].job_pid);
	}
	return;
}
