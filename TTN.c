#include "./TTN.h"

//extern 초기화
TTNode * pro_dir_root = NULL;
TTNode * pro_decoy_root = NULL;
TTNode * pro_extentions_root = NULL;

int TTN_add(TTNode ** root, char * data)
{
	if( (*root) == NULL)
	{
		*root = (TTNode *)kmalloc(sizeof(TTNode), GFP_KERNEL);
		if( (*root) == NULL) return -1;
		(*root)->left = (*root)->down = (*root)->right = NULL;
		(*root)->data = (*data);
		(*root)->endFlag = 0;
	}

	if( (*(data + 1)) == '\0' && (*root)->data == (*data))
	{
		(*root)->endFlag = 1;
		return 0;
	}

	if( (*root)->data > (*data) )
		TTN_add(&(*root)->left, data);
	else if( (*root)->data == (*data) )
		TTN_add(&(*root)->down, data + 1);
	else
		TTN_add(&(*root)->right, data);
	
	return 0;
}

void TTN_deleteAll(TTNode ** root)
{
	if( (*root) == NULL) return;
	TTN_deleteAll( &(*root)->left);
	TTN_deleteAll( &(*root)->down);
	TTN_deleteAll( &(*root)->right);

	kfree( (*root) );
}

int TTN_search(TTNode * root, char * search_str, int dir_flag)
{
	if(root == NULL)
		return -1;

	if(root->data > *search_str)
		return TTN_search(root->left, search_str, dir_flag);
	else if(root->data < *search_str)
		return TTN_search(root->right, search_str, dir_flag);
	else
	{
		if(root->endFlag && (*(search_str + 1) == '\0'))
			return 0;
		//pro_dir_root 의 하위 디렉터리 접근 방지
		if(dir_flag)
			if(root->endFlag && (*(search_str + 1) == '/'))
				return 0;
		return TTN_search(root->down, ++search_str, dir_flag);
	}
	return 0;
}


/////////////////////테스트용 함수 
char buf[100];
int buf_index = 0;
void TTN_printAll(TTNode * root)
{
	if(root == NULL) return ;
	TTN_printAll(root->left);
	buf[buf_index] = root->data;
	if(root->endFlag)
	{
		buf[buf_index + 1] = '\0';
		printk("[Tree module] : %s\n", buf);
	}
	buf_index++;
	TTN_printAll(root->down);
	buf_index--;
	TTN_printAll(root->right);
}
