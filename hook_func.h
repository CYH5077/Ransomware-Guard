#ifndef __GUARD_OPEN_HOOK_HEADER__
#define __GUARD_OPEN_HOOK_HEADER__

#include <linux/syscalls.h>
#include <linux/fs_struct.h>
#include "./TTN.h"
#include "./file.h"
#include "./log.h"
#define CR0_PROTECTION 0x00010000


//기존 주소 저장.
extern asmlinkage int(*real_sys_open)(const char *, int, int);
extern asmlinkage ssize_t(*real_sys_write)(int, const void *, size_t);
extern asmlinkage int (*real_sys_rename)(const char *, const char *);
//새로운 sys_write
asmlinkage ssize_t my_sys_write(int fd, const void * buf, size_t count);
asmlinkage int my_sys_open(const char * file_path, int flags, int mode);
asmlinkage int my_sys_rename(const char *oldpath, const char *newpath);

//Syscall 테이블 주소 저장
extern void ** sys_table;

/* 
 * syscall table의 주소를 찾아 sys_table 에 저장.
 * 성공시 0
 * 실패시 -1 반환
 */
int find_sys_table(void);

//Syscall Hooking
void hook_start(void);
void hook_end(void);

#endif
