#ifndef __RANSOM_GUARD_TTN_HEADER__
#define __RANSOM_GUARD_TTN_HEADER__

#include <linux/slab.h> //kmalloc(), kfree()


//삼진 노드 구조체
typedef struct TripleTreeNode{
	char data;
	short endFlag;

	struct TripleTreeNode *left, *down, *right;
} TTNode;

extern TTNode * pro_dir_root;		// PRO_DIR 루트 노드
extern TTNode * pro_decoy_root;		// PRO_DECOY 루트 노드
extern TTNode * pro_extentions_root;	// PRO_EXTENTIONS_ROLLBACK 루트 노드


/*
 * TTN 노드에 데이터를 추가한다. (TTN.c)
 * root - TTN 루트 노드, data - 추가할 문자열
 * 성공시 0 반환
 * 실패시 -1 반환
 */
int TTN_add(TTNode ** root, char * data);

/*
 * TTN 노드를 전부 제거한다. (TTN.c)
 * root - TTN 루트 노드
 */
void TTN_deleteAll(TTNode ** root);

/*
 * TTN 노드에서 데이터를 검색한다. (TTN.c)
 * root - 검색할 루트 노드, search_str - 검색할 데이터를, dir_flag - 디렉토리 체크 옵션
 * 찾았을 경우 0 반환
 * 못찾았을 경우 -1 반환
 */
int TTN_search(TTNode * root, char * search_str, int dir_flag);

//출력 테스트용
void TTN_printAll(TTNode * root);
#endif
