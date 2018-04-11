#ifndef __RANSOM_GUARD_PARSE_HEADER__
#define __RANSOM_GUARD_PARSE_HEADER__

#include <linux/fs.h> //strcpy
#include "./TTN.h"

//헤더 타입 정의
typedef enum SET_HEADER{
	PRO_ERROR = 0,
	PRO_ADD_ERROR,
	PRO_DIR,
	PRO_DECOY,
	PRO_EXTENTIONS_ROLLBACK
} set_val;

/*
 * guard_write() 에서 사용될 함수로 set_val 의 값중 하나를 반환한다. (parse.c)
 * buf - guard_write() 에서 넘어온 데이터 버퍼
 * 성공시 set_val 의 값을 반환한다.
 * 실패시 PRO_ERROR 반환
 */
set_val prs_data(char * buf);

/*
 * 해당되는 루트노드 에 데이터를 추가한다. (parse.c)
 * header_type - set_val 값중 하나, add_data 데이터 정보
 * 성공시 0 반환
 * 실패시 -1 반환
 */
int prs_add_data(set_val header_type, char * add_data);

#endif 
