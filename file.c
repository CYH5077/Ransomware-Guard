#include "./file.h"

//extern 초기화
struct file_operations guard_event;
int reg_guard_chr(int dev_num, char * name)
{
	guard_event.open = guard_open;
	guard_event.release = guard_close;
	guard_event.write = guard_write;
	guard_event.read = guard_read;

	if(register_chrdev(dev_num, name, &guard_event) < 0)
		return -1;
	return 0;
}
void unreg_guard_chr(int dev_num, char * name)
{
	unregister_chrdev(dev_num, name);
}
//chr open
int guard_open(struct inode * inode, struct file * file_st)
{
	return 0;
}

//chr write
ssize_t guard_write(struct file * file_st, const char * buf, size_t count, loff_t *f_pos)
{
	char *temp_buf = (char *)kmalloc(count + 1, GFP_KERNEL);
	set_val ret_val;

	strcpy(temp_buf, buf);
	ret_val = prs_data(temp_buf);
	kfree(temp_buf);
	if(ret_val == PRO_ERROR || ret_val == PRO_ADD_ERROR)
		return -1;
	return 0;
}
//chr close
int guard_close(struct inode * inode, struct file * file_st)
{
	return 0;
}
//chr read
ssize_t guard_read(struct file *file_st, char *buf, size_t count, loff_t *f_pos)
{
	return 0;
}


int send_killSig(struct task_struct * task)
{
	struct siginfo info;

	memset(&info, 0x00, sizeof(info));
	info.si_signo = SIGKILL;

	if(send_sig_info(SIGKILL, &info, task) < 0)
		return -1;
	return 0;
}

int get_symlink_file(const char *link_file, char *real_file, int buf_size)
{
	struct path link_path;
	char temp_buf[256] = {0,};
	char * result;

	if(kern_path(link_file, LOOKUP_FOLLOW, &link_path) < 0)
		return -1;

	result = d_path(&link_path, temp_buf, 256);
	if(IS_ERR(result))
		return -1;

	if(strlen(result) > buf_size)
		return -1;
	strcpy(real_file, result);
	return 0;
}


// 내생각엔 딱히 실패할 경우가 없을것 같다......
// 반환형이 쓸모없을지도.
int path_maker(char * path, char * full_path, int buf_size)
{
	struct path work_path;
	char * chroot_path, chroot_path_temp[256] = {0,};

	strcpy(full_path, path);
	if(*path != '/')
	{
		//프로세스의 작업 경로를 얻어온다.
		work_path = current->fs->pwd;
		chroot_path = d_path(&work_path, chroot_path_temp, 256);
		strcpy(full_path, chroot_path);
		if(strncmp(path, "./", 2) == 0) // ./dir 과 같은 형식
		{
			path++; //앞의 . 제거
			full_path = full_path + strlen(chroot_path) + 1;
		}
		else // dir/dir 과 같은 형식
		{
			full_path[strlen(chroot_path) + 1] = '/';
			full_path[strlen(chroot_path) + 2] = '\0';
			full_path = full_path + strlen(chroot_path) + 2;
		}
		strcpy(full_path, path);
	}
	return 0;
}

int check_extentions(char * path)
{
	int loop;
	char * path_extention;
	
	for(loop = strlen(path); loop >= 0; loop--)
		if(path[loop] == '.' || path[loop] == '/')
			break;
	
	if(path[loop++] == '.')
	{
		path_extention = path + loop;
		return TTN_search(pro_extentions_root, path_extention, 0);
	}
	return -1;
}

static void make_backup_name(char *target, char * backup)
{
	char date_buf[50];
	char * target_fname;
	int loop;
	for(loop = strlen(target) - 1 ; loop >= 0; loop--)
		if(target[loop] == '/')
			break;
	target_fname = target + loop + 1;
	get_time(date_buf);
	sprintf(backup, "%s/%d_%s_%s_guard",  backup_path, current->pid, date_buf, target_fname);
}
//마음에 안듬 나중에 수정 예정
int extentions_backup(char * real_file)
{
	struct file * target_file,* backup_file;
	char buf[512];
	char backup_name[256];
	int ret_val;
	mm_segment_t old = get_fs();
	
	if(backup_path != NULL)
		make_backup_name(real_file, backup_name);
	else return -1;

	set_fs(KERNEL_DS);
	
	target_file = filp_open(real_file, O_RDONLY, 0);
	if(IS_ERR(target_file)){
		set_fs(old);
		return -1;
	}
	ret_val = vfs_read(target_file, buf, 512, &target_file->f_pos);
	if(ret_val <= 0)
	{
		set_fs(old);
		filp_close(target_file, NULL);
		return 0;
	}

	backup_file = filp_open(backup_name, O_RDWR | O_CREAT, 0);
	if(IS_ERR(backup_file)){
		filp_close(target_file, NULL);
		set_fs(old);
		return -1;
	}
	vfs_write(backup_file, buf, ret_val, &backup_file->f_pos);
	while(1)
	{
		ret_val = vfs_read(target_file, buf, 512, &target_file->f_pos);
		if(ret_val != 0) //내용이 없는 파일 생성시
			vfs_write(backup_file, buf, ret_val, &backup_file->f_pos);
		if(ret_val < 512)
			break;
	}

	filp_close(target_file, NULL);
	filp_close(backup_file, NULL);
	set_fs(old);

	return 0;
}
