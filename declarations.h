#define clear() printf("\033[H\033[J")
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET "\e[m"
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
int proc_type[100000];
pid_t global_pid, shell_pid;
char name[100000][20];
char foreground_proc[100000][30];
char commands[1000][1000];
int no_of_commands = 0;