#ifndef __RANSOMGAURD_USER_PARSE_HEADER__
#define __RANSOMGAURD_USER_PARSE_HEADER__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

//데이터 타입.
typedef enum DATA_TYPE {
	CONF_ERROR = 0, 	//설정 파일 에러
	CONF_HEADER,		//설정 헤더
	CONF_DATA,		//설정 데이터.
	CONF_DESCRIPTION	//주석
} data_type;

// 헤더 종류
typedef enum SETTING_VALUE {
	HEADER_ERROR = 0,	//헤더 데이터 오류.
	SETTING_LOG_PATH,	// #SETTING_LOG_PATH
	SETTING_PATH,		// #SETTING_PATH 
	SETTING_BACKUP_PATH,	// #SETTING_BACKUP_PATH
	PRO_DIR,		// #PRO_DIR
	PRO_DECOY,		// #PRO_DECOY
	PRO_EXTENTIONS_ROLLBACK	// #PRO_EXTENTIONS_ROLLBACK
} set_val;

//설정 데이터 관련 
typedef struct Setting{
	FILE * conf_fd;		//설정파일 스트림
	set_val hdr_type;	//설정값 타입. (헤더 타입)
	int line;		//설정파일 에러시 에러 발생라인 출력용
	char data[256];		//한줄에 공백으로 연속 데이터가 있으면 저장용 임시 버퍼
	char * set_path;	//설정파일 경로.

	int set_log_flag, set_backup_flag; //insmod
} config_data;

/*
 * 설정파일 open (parse.c)
 * conf - config_data 구조체 주소, file - 설정파일 경로
 * 성공시 0
 * 실패시 -1
 */
int prs_open(config_data * conf, char * file);

/*
 * 설정파일 닫기(parse.c)
 * conf - config_data 구조체
 */
void prs_close(config_data * conf);

/*
 * line의 타입 반환. (parse.c)
 * conf - config_data 구조체 주소, line - char 버퍼
 * 성공시 CONF_HEADER(5), CONF_DATA(6) 반환
 * 실패시 CONF_ERROR(0)
 */
data_type prs_check_line(config_data * conf, char * line);

#define GET_ARG 1
/*
 * 한줄 읽고 파싱 데이터를 반환 (parse.c)
 * conf - config_data 구조체, arg_flag - SETTING_LOG_PATH, SETTING_BACKUP_PATH 에 대한 정보만을 가져올 때 GET_ARG 를 넘겨준다. 아니면 0
 * 성공시 0 반환 파일의 끝이면 1반환
 * 실패시 실패한 라인  반환.
 */
int prs_get_data(config_data * conf, int arg_flag);

#endif
