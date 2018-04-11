#ifndef __RANSOMWARE_GUARD_FILE__
#define __RANSOMWARE_GUARD_FILE__
#include "parse.h"
#include "log.h"
#include <linux/file.h>
#include <linux/namei.h>
#include <linux/syscalls.h> //시그널 떄문
#include <linux/fs.h>//strcpy
#include <linux/fs_struct.h> //path_maker 에서 file 로

//===================================================== chr_device
#define DEVICE_NUM 2000
extern struct file_operations guard_event;
int guard_open(struct inode * inode, struct file * file_st);
ssize_t guard_write(struct file * file_st, const char * buf, size_t count, loff_t *f_pos);
int guard_close(struct inode * inode, struct file *file_st);
ssize_t guard_read(struct file *file_st, char *buf, size_t count, loff_t *f_pos);
/*
 * 유저영역의 프로세스와 통신할 문자 드라이버 등록 (file.c)
 * dev_num - 주 번호, name - 디바이스 명칭
 * 성공시 0을 반환하고
 * 실패시 -1을 반환
 */
int reg_guard_chr(int dev_num, char * name);
/*
 * 문자 디바이스 드라이버 해제 (file.c)
 * dev_num - 주 번호, name - 디바이스 명칭
 */
void unreg_guard_chr(int dev_num, char * name);

//===================================================== chr_device


/*
 * task 프로세스에 SIGKILL 시그널을 전송한다 (file.c)
 * task - 종료할 프로세스의 task_struct 구조체 포인터
 * 성공시 0을 반환하고
 * 실패시 -1을 반환
 */
int send_killSig(struct task_struct * task);

/*
 * 링크 파일이 가리키는 파일의 경로를 가져온다. (file.c)
 * link_file - 심볼릭 링크파일 경로, real_file - link_file 의 실제 경로를 저장할 버퍼
 * buf_size - real_file버퍼의 크기.
 * 성공시 0 반환
 * 싪패시 -1반환
 */
int get_symlink_file(const char *link_file, char *real_file, int buf_size);

/*
 * 경로처리 함수 (./dir dir/dir 와 같은 경로를 full 경로로 바꾼다.) (file.c)
 * path - ./ or dir/dir 과 같은 경로, full_path - 풀경로를 반환받을 버퍼
 * buf_size - full_path 버퍼의 크기
 * 성공시 0
 * 실패시 -1을 반환
 */
int path_maker(char * path, char * full_path, int buf_size);

/*
 * 확장자 체크 pro_extentions_root 검색 (file.c)
 * path - 확장자를 체크할 파일 
 * 해당되지 않는 확장자의 경우 -1 반환
 * 해당 확장자가 보호되어야 할경우 0 반환
 */
int check_extentions(char * path);

/*
 * Extentions 확장자에 대해 백업을 수행하는 함수 (file.c)
 * real_file - 백업할 파일.
 */
int extentions_backup(char * real_file);

#endif
