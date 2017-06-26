#include "./hook_func.h"

//extern 초기화
asmlinkage int(*real_sys_open)(const char *, int, int) = NULL;
asmlinkage ssize_t(*real_sys_write)(int, const void *, size_t) = NULL;
void ** sys_table = NULL;


int find_sys_table(void)
{
	unsigned long table, *ptr;
	for(table = (unsigned long)sys_close; ; table += sizeof(void*))
	{
		ptr = (unsigned long*)table;
		if(ptr[__NR_close] == (unsigned long) sys_close)
		{
			sys_table = (void **)ptr;
			return 0;
		}
	}
	return -1;
}

void hook_start(void)
{
	unsigned long cr0 = read_cr0();
	
	write_cr0(cr0 & ~CR0_PROTECTION);

	//sys_open()
	real_sys_open = sys_table[__NR_open];
	sys_table[__NR_open] = my_sys_open;
	//sys_write()
	real_sys_write = sys_table[__NR_write];
	sys_table[__NR_write] = my_sys_write;

	write_cr0(cr0);
}

void hook_end(void)
{
	unsigned long cr0 = read_cr0();

	write_cr0(cr0 & ~CR0_PROTECTION);

	//sys_open(), sys_write() 원래 주소로 복구
	sys_table[__NR_open] = real_sys_open;
	sys_table[__NR_write] = real_sys_write;
	
	write_cr0(cr0);
}


asmlinkage int my_sys_open(const char * file_path, int flags, int mode)
{
	char full_path[256], copy_file_path[256];
	char proc_path[100] = {0,};
	char my_real_path[256] = {0,};
	char * full_temp;

	strcpy(copy_file_path, file_path);
	path_maker(copy_file_path, full_path, 256);

	if(strstr(file_path, "00000"))
		//보호되는 디렉토리인지 체크
		printk(KERN_INFO "[MODULE]:%s\n", full_path);
	if(!TTN_search(pro_dir_root, full_path, 1))
	{
		//현재 함수를 호출한 프로세스의 파일 풀 경로.
		sprintf(proc_path, "/proc/%d/exe", current->pid);
		get_symlink_file(proc_path, my_real_path, 256);
		
		log_write(my_real_path, full_path, "ACCESS - PRO_DIR");	
		send_killSig(current);
		return -1;
	}
	//경로에 파일명 추가
	full_temp = full_path + strlen(full_path);
	full_temp[0] = '/'; full_temp[1] = '\0'; full_temp++;
	strcpy(full_temp, copy_file_path);
	
	//확장자 체크
	if(!check_extentions(full_path))
	{
		if(flags | O_WRONLY || flags | O_RDWR)
		{ //권한체크
			log_write(my_real_path, full_path, "WRITE_ACCESS - PRO_EXTENTIONS");
			extentions_backup(full_path);
		}
	}

	//현재 sys_open 을 호출한 프로세스 종료 send_killSig(current);
	return real_sys_open(file_path, flags, mode);
}

asmlinkage ssize_t my_sys_write(int fd, const void * buf, size_t count)
{
	char proc_path[100] = {0,};
	char target_real_path[256], my_real_path[256];

	//현재 fd 로 넘어온 파일의 풀 경로를 받아온다.
	sprintf(proc_path, "/proc/%d/fd/%d", current->pid, fd);
	get_symlink_file(proc_path, target_real_path, 256);
	
	//현재 함수를 호출한 프로세스의 파일 풀 경로
	sprintf(proc_path, "/proc/%d/exe", current->pid);
	get_symlink_file(proc_path, my_real_path, 256);
	
	//Decoy 파일인지 확인.
	if(!TTN_search(pro_decoy_root, target_real_path, 0))
	{	
		//우선 사용자 명령은 가능하게
		//나중에 예외 프로그램 추가 예정.
		//사용자 명령으로 수정은 가능하도록 (vi 같은)
		if(strncmp(my_real_path, "/usr/bin", 7) != 0)
		{
			log_write(my_real_path, target_real_path, "WRITE - DECOY");
			send_killSig(current);
			return -1;
		}
	}
	//확장자 체크
	if(!check_extentions(target_real_path)){
		log_write(my_real_path, target_real_path, "WRITE - PRO_EXTENTIONS");
	}

	return real_sys_write(fd, buf, count);
}
