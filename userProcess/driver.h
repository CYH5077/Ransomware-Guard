#ifndef __RANSOMGUARD_USER_DRIVER_HEADER__
#define __RANSOMGUARD_USER_DRIVER_HEADER__
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include "./parse.h"

#define DEVICE_NUM 2000

typedef struct USER_DRIVER{
	config_data setting;	//설정 관련 parse.h 정의
	int chr_fd;		//문자 디바이스 파일 디스크립터.
	char chr_driver[256];	//문자 디바이스 경로.
} guard_driver;

/*
 * 문자 디바이스 파일을 생성한다 (driver.c)
 * dirver - guard_driver 구조체 포인터, drive_name - 드라이버 명
 * drive_path - 문자디바이스 생성 경로, conf_path - guard.conf 파일 경로
 * 성공시 0 반환
 * 실패시 -1 반환
 */
int driver_ready(guard_driver * driver, char * drive_name, char * drive_path, char * conf_path);

/*
 * 열려있는 파일을 닫고 unlink (driver.c)
 * driver - guard_driver 구조체 포인터.
 */
void driver_close(guard_driver * driver);

/*
 * 데이터를 드라이버에 쓴다. (driver.c)
 * driver - guard_driver 구조체 포인터
 * 성공시 0
 * 실패시 설정 파일의 잘못된 라인을 반환
 */
int driver_write(guard_driver * driver);
#endif
