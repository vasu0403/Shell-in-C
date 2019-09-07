const int max_len = 1e5, max_path = 1e4;
int no_of_jobs = 0;
char home_dir[100000];
struct job_struct {
	int job_number;
	char job_name[100];
	char job_status[100];
	int job_pid;
};
struct job_struct all_jobs[100000];
int job_pid_to_job_number[100000];