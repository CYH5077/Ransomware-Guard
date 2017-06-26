#include "./log.h"


char * log_path = NULL;
struct file * log_file = NULL;
short is_log_open = 0;
char * backup_path = NULL;

void get_time(char * buf)
{
	unsigned long get_time;
	struct timeval t_v;
	struct tm t_m;

	do_gettimeofday(&t_v);
	get_time = t_v.tv_sec;

	time_to_tm(get_time, 0, &t_m);

	sprintf(buf, "[%ld-%d-%d %d:%d:%d] ",t_m.tm_year + 1900,
					     t_m.tm_mon + 1,
					     t_m.tm_mday,
					     t_m.tm_hour + 9,
					     t_m.tm_min,
					     t_m.tm_sec);
}

int log_open(void)
{
	mm_segment_t old = get_fs();
	set_fs(KERNEL_DS);

	log_file = filp_open(log_path, O_RDWR | O_CREAT, 0);
	if(IS_ERR(log_file))
		return -1;

	set_fs(old);
	is_log_open = 1;
	return 0;
}

void log_write(char * my_path, char * file_path, char * log_data)
{
	char log_buf[300] = {0,};
	char *log_temp;
	mm_segment_t old = get_fs();
	set_fs(KERNEL_DS);

	get_time(log_buf);
	log_temp = log_buf + strlen(log_buf);

	sprintf(log_temp, "%s [%s]:::[%s]\n", log_data, my_path, file_path);
	vfs_write(log_file, log_buf, strlen(log_buf), &log_file->f_pos);
	
	set_fs(old);
}

void log_close(void)
{
	mm_segment_t old = get_fs();
	if(is_log_open){
		set_fs(KERNEL_DS);
		filp_close(log_file, NULL);
		set_fs(old);
	}
}
