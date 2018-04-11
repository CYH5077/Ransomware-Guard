#ifndef __RANSOMGUARD_LOG_HEADER__
#define __RANSOMGUARD_LOG_HEADER__
#include <linux/moduleparam.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/fd.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>

#include <linux/time.h>

extern char * log_path; //로그파일 경로
extern short is_log_open; //로그파일이 열렸는지 log_close 시에러 떄문
extern struct file  * log_file; //로그 파일
extern char * backup_path; //PRO_EXTENTIONS 의 확장자 파일 내용 write 권한으로 open 시  백업 폴더

/*
 * 현재 시간을 받아온다. (log.c)
 * buf - 시간정보를 받아올 버퍼
 */
void get_time(char * buf);

/*
 * 로그파일을 연다 (log.c)
 * 성공시 0
 * 실패시 -1 반환
 */
int log_open(void);

/*
 * 로그파일에 쓴다 (log.c)
 * log_data - 쓸 내용
 * 성공시 0 
 * 실패시 -1 반환
 */
void log_write(char * my_path, char * file_path, char * log_data);

void log_change(char * name_change, int start_index, int end_index, char * real_path);
/*
 * 로그 파일을 닫는다 (log.c)
 */
void log_close(void);
#endif
