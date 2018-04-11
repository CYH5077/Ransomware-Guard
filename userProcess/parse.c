#include "./parse.h"

int prs_open(config_data * conf, char * file)
{
	//초기화.
	conf->line = 0;
	conf->set_path = NULL;

	conf->conf_fd = fopen(file, "r");
	if(conf->conf_fd == NULL)
		return -1;
	//경로저장
	conf->set_path = (char *)malloc(strlen(file)+1);
	strcpy(conf->set_path, file);
	return 0;
}

void prs_close(config_data * conf)
{
	if(conf->set_path != NULL)
		free(conf->set_path);
	fclose(conf->conf_fd);
}

//헤더의 종류 반환
static set_val find_header_data(char * data)
{
	if(strcmp(data, "SETTING_LOG_PATH\n") == 0)
		return SETTING_LOG_PATH;
	else if(strcmp(data, "SETTING_PATH\n") == 0)
		return SETTING_PATH;
	else if(strcmp(data, "SETTING_BACKUP_PATH\n") == 0)
		return SETTING_BACKUP_PATH;
	else if(strcmp(data, "PRO_DIR\n") == 0)
		return PRO_DIR;
	else if(strcmp(data, "PRO_DECOY\n") == 0)
		return PRO_DECOY;
	else if(strcmp(data, "PRO_EXTENTIONS_ROLLBACK\n") == 0)
		return PRO_EXTENTIONS_ROLLBACK;
	return HEADER_ERROR;
}

data_type prs_check_line(config_data * conf, char * line)
{
	if(*line == '#')
	{
		conf->hdr_type = find_header_data(line+1);;
		return CONF_HEADER;
	}
	else if(strncmp(line, "//", 2) == 0)
		return CONF_DESCRIPTION;
	else if(*line == '/' || *line == '.')
		return CONF_DATA;
	return CONF_ERROR;
}

int prs_get_data(config_data * conf, int get_arg)
{
	char read_buf[256] = {0, };
	char * read_temp;
	data_type temp_type;

	while(1)
	{
		if(fgets(read_buf, 256, conf->conf_fd) == NULL) 
			return 1;
		conf->line++;
		
		if(*read_buf == '\n')
			continue;//빈줄 통과

		temp_type = prs_check_line(conf, read_buf);
		if( (temp_type == CONF_DESCRIPTION) || 
		    (temp_type == CONF_HEADER) )
			continue;//주석, 헤더 통과
		
		if(get_arg) //GET_ARG
		{
			if(conf->hdr_type == SETTING_LOG_PATH ||
			   conf->hdr_type == SETTING_BACKUP_PATH)
				break;
			else
				continue;
		}
		else
			break;
	}
	//읽은 데이터 타입.
	if(temp_type == HEADER_ERROR)
		return conf->line;
	if((temp_type == CONF_DATA) && (conf->hdr_type == HEADER_ERROR))
		return conf->line;
	

	//read_buf 에 맨앞 './' 혹은 경로가 dir/dir 형식을 처리하기위한
	//작업을 추가해야한다.
	//이곳에
	
	read_buf[strlen(read_buf) - 1] = '\0';	
	if(conf->hdr_type == PRO_EXTENTIONS_ROLLBACK){
		read_temp = read_buf+1;
		strcpy(conf->data, read_temp);
		return 0;
	}
	strcpy(conf->data, read_buf);
	return 0;
}
