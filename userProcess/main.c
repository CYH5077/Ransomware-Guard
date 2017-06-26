#include <stdio.h>
#include "./driver.h"

static int start_guard(char * drive_name, char * drive_path, char * conf)
{
	guard_driver driver;
	int err_line;

	if(driver_ready(&driver, drive_name, drive_path, conf) == -1)
	{
		perror("driver_ready() failed");
		return -1;
	}

	err_line = driver_write(&driver);
	if(err_line == -1)
	{
		perror("driver_write() failed");
		driver_close(&driver);
		return -1;
	}
	else if(err_line)
	{
		printf("%d line error \n", err_line);
		driver_close(&driver);
		return -1;
	}
		

	driver_close(&driver);
	return 0;
}

#define INSMOD		0x00000001
#define D_OPTION 	0x00000010
#define C_OPTION 	0x00000100
#define S_OPTION 	0x00001000
#define FULL_OPTION	0x00001111

#define RMMOD		0x00000000

int main(int argc, char * argv[])
{
	int result;
	char run_mode = 0;
	char dev_path[256] = {0,}, 
	     chr_dev_path[256] = {0,}, 
	     conf_path[256] = {0,}; 
	
	if(argc < 2)
	{
		printf("Options \n"
			"-i [start]\n"
			"-r [stop] \n"
			"-d 'path' [Module path]\n"
			"-c 'path' [Chr Device path]\n"
			"-s 'path' [Config file path]\n");
		return -1;
	}
	while((result = getopt(argc, argv, "ird:c:s:")) != -1)
	{
		printf("%c: %s\n", result, optarg);
		switch(result)
		{
			case 'i': //insmod
				run_mode |= INSMOD;
				break;
			case 'r': //rmmod
				run_mode |= RMMOD;
				break;
			case 'd': //모듈 파일
				run_mode |= D_OPTION;
				strcpy(dev_path, optarg);
				break;
			case 'c': //문자 디바이스 경로
				run_mode |= C_OPTION;
				strcpy(chr_dev_path, optarg);
				break;
			case 's': //conf 파일 경로
				run_mode |= S_OPTION;
				strcpy(conf_path, optarg);
				break;
		}
	}

	if(run_mode == FULL_OPTION)
		return start_guard(dev_path, chr_dev_path, conf_path);
	
	
	return 0;
}
