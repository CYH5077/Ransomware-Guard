#include "./parse.h"

//문자  커팅 함수
static char * my_linecut(char * cut_str, const char cut_token, char ** next_line)
{
	int loop;
	int end_str;
	char * ret_buf = (char *)kmalloc(sizeof(char) * 256, GFP_KERNEL);

	if(*next_line != NULL)
		cut_str = *next_line;

	end_str = strlen(cut_str);
	for(loop = 0; loop < end_str || loop > 254; loop++)
	{
		if(cut_str[loop] == cut_token)
			break;
		ret_buf[loop] = cut_str[loop];
	}

	if(cut_str[loop + 1] == '\0')
	{
		*next_line = NULL;
		ret_buf[loop] = '\0';
		return ret_buf;
	}

	*next_line = cut_str + loop + 1;
	ret_buf[loop] = '\0';
	return ret_buf;
}

static set_val get_header_type(char * header_data)
{
	if(strcmp(header_data, "PRO_DIR") == 0)
		return PRO_DIR;
	else if(strcmp(header_data, "PRO_DECOY") == 0)
		return PRO_DECOY;
	else if(strcmp(header_data, "PRO_EXTENTIONS_ROLLBACK") == 0)
		return PRO_EXTENTIONS_ROLLBACK;
	return PRO_ERROR;
}


set_val prs_data(char * buf)
{
	char *header_data, *next_line = NULL;
	set_val header_type;	
	
	//헤더 추출
	header_data = my_linecut(buf, '\n', &next_line);
	if(next_line == NULL)
		return PRO_ERROR;

	header_type = get_header_type(header_data);

	if(header_type == PRO_ERROR)
		return PRO_ERROR;

	if(prs_add_data(header_type, next_line))
		return PRO_ADD_ERROR;

	return header_type;
}

//prs_add_data() 에서 해당되는 root 노드에 데이터를 추가한다.
//성공시 0, 하나라도 실패시 -1
static int add_TTN_data(TTNode ** root, char * data)
{
	char *cut_data, *next_line = NULL;
	cut_data = my_linecut(data, '\n', &next_line);
	while(next_line != NULL)
	{
		if(TTN_add(root, cut_data)) return -1;
		kfree(cut_data);
		cut_data = my_linecut(data, '\n', &next_line);
	}
	if(TTN_add(root, cut_data)) return -1;
	kfree(cut_data);

	return 0;
}

int prs_add_data(set_val header_type, char * add_data)
{
	switch(header_type)
	{
		case PRO_DIR :
			return add_TTN_data(&pro_dir_root, add_data);
		case PRO_DECOY :
			return add_TTN_data(&pro_decoy_root, add_data);
		case PRO_EXTENTIONS_ROLLBACK : 
			return add_TTN_data(&pro_extentions_root, add_data);
		default:
			break;
	}
	return PRO_ERROR;
}
