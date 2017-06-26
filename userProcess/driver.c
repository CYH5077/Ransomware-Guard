#include "./driver.h"

//driver insmod 함수.
static int insmod(char * drive_name, char * arg)
{
	int drive_fd = open(drive_name, O_RDONLY);
	struct stat drive_st;

	int driver_size;
	void * driver_buf;

	if(drive_fd == -1) 
		return -1;

	fstat(drive_fd, &drive_st);
	driver_size = drive_st.st_size;
	driver_buf = (void *)malloc(driver_size);

	if(read(drive_fd, driver_buf, driver_size) == -1)
	{
		close(drive_fd); free(driver_buf);
		return -1;
	}
	if(syscall(__NR_init_module, driver_buf, driver_size, arg) != 0)
	{
		close(drive_fd); free(driver_buf);
		return -1;
	}
	free(driver_buf);
	return 0;
}

//driver insmod 시 넘길 인자 생성 함수
//성공시 0
//실패시 -1
static int make_arg(config_data * conf, char * arg_buf)
{
	char * data_offset = NULL;
	data_offset = arg_buf + strlen(arg_buf);
	switch(conf->hdr_type)
	{
		case SETTING_LOG_PATH :
			conf->set_log_flag = 1;
			sprintf(data_offset, "log_path=\"%s\" ", conf->data);
			break;
		case SETTING_BACKUP_PATH:
			conf->set_backup_flag = 1;
			sprintf(data_offset, "backup_path=\"%s\" ", conf->data);
			break;
		default:
			return -1;
	}
	return 0;
}

//drive시작시 넘길 인자를 생성한 후 insmod 함수에 넘긴다.
//성공시 0 
//실패시 -1 반환
static int driver_start(char * drive_name, char * conf_path)
{
	config_data conf;
	int ret_val;
	int set_log_flag = 0, set_backup_flag = 0;
	char arg_buf[300] = {0,};

	if(prs_open(&conf, conf_path) == -1)
		return -1;
	while(1)
	{
		ret_val = prs_get_data(&conf, GET_ARG);
		if(ret_val == 1) break; //EOF

		if(ret_val > 0) //ERROR
		{
			prs_close(&conf);
			return -1;
		}
		
		make_arg(&conf, arg_buf);
	}
	prs_close(&conf);
	if(conf.set_log_flag && conf.set_backup_flag)
		return insmod(drive_name, arg_buf);
	return -1;
}

int driver_ready(guard_driver * driver, char * drive_name, char * drive_path, char * conf_path)
{
	if(driver_start(drive_name, conf_path) == -1)
		return -1;
	strcpy(driver->chr_driver, drive_path);
	
	if(mknod(driver->chr_driver, S_IFCHR, (DEVICE_NUM << 8) | 1) != 0)
		return -1;
	
	driver->chr_fd = open(driver->chr_driver, O_WRONLY, 0);
	if(driver->chr_fd == -1)
	{
		unlink(driver->chr_driver);
		return -1;
	}
	if(prs_open(&driver->setting, conf_path) == -1)
	{
		driver_close(driver);
		return -1;
	}
	return 0;
}

void driver_close(guard_driver * driver)
{
	close(driver->chr_fd);
	unlink(driver->chr_driver);
}

//드라이버로 전달할 데이터 생성.
//buf - 전달할 내용을 저장할 버퍼, buf_size - buf의 크기
//driver - guard_driver 구조체 포인터
static int make_data(char * buf, int buf_size, guard_driver * driver)
{
	char * buf_offset = NULL;
	memset(buf, 0x00, buf_size);
	switch((driver->setting).hdr_type)
	{
		case PRO_DIR:
			strcpy(buf, "PRO_DIR\n"); break;
		case SETTING_PATH: //설정파일도 Decoy 로 등록
		case PRO_DECOY:
			strcpy(buf, "PRO_DECOY\n"); break;
		case PRO_EXTENTIONS_ROLLBACK:
			strcpy(buf, "PRO_EXTENTIONS_ROLLBACK\n"); break;
		default:
			return -1;
	}
	buf_offset = buf + strlen(buf);
	strcpy(buf_offset, (driver->setting).data);
	return 0;
}
int driver_write(guard_driver * driver)
{
	int ret_val;
	char buf[300] = {0,};
	while(1)
	{
		ret_val = prs_get_data(&driver->setting, 0);
		if(ret_val == 1)//EOF
			break;
		if(ret_val > 0)
		{//ERROR
			prs_close(&driver->setting);
			return ret_val;
		}
		if(!make_data(buf, 300, driver))
		{
			if(write(driver->chr_fd, buf, strlen(buf)) == -1)
			{
				prs_close(&driver->setting);
				return -1;
			}
		}
	}
	prs_close(&driver->setting);
	return 0;
}
