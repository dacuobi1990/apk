#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#define CONFIG_NAME "config.ini"

#define BUFF_SIZE 1024
#define TRUE 1
#define FALSE 0

match_info soft_array[SOFT_MAX_NUM];
int soft_num=0;



static int judge_head(char * s,int len)
{
	if(len<=3)
		return FALSE;
	if(s[0]=='[' && s[len-2]==']')
		return TRUE;
	else
		return FALSE;
}

static int find_symbol(char * line)
{
	int i;
	int len = strlen(line);
	for(i=0;i<len;i++)
	{
		if(line[i]=='=')
			break;
	}
	if(i==len)
		return -1;
	return i;
}

static int is_equal(const char * aim,const char * base)
{
	int i;
	for(i=0;i<strlen(base);i++)
	{
		if(aim[i]!=base[i])
			return FALSE;
	}
	return TRUE;
}

static void m_strcpy(char * dst,char *src,int l,int r)
{
	int i;
	int j=0;
	for(i=l;i<=r;i++)
		dst[j++]=src[i];
	dst[j]='\0';
}

void ini_purse()
{
	char line_buf[BUFF_SIZE];
	int buf_len =0;
	FILE * fp = fopen(CONFIG_NAME,"r");
	if(fp == NULL)
	{
		fprintf(stderr, "%s,%d,%s\n",__FILE__,__LINE__,"open config file error" );
		return;
	}	

	while(!feof(fp))
	{
		memset(line_buf,0,sizeof(line_buf));
		fgets(line_buf,1024,fp);
		buf_len = strlen(line_buf);
		if(judge_head(line_buf,buf_len)==TRUE)
		{
			soft_num++;
			soft_array[soft_num-1].name=(char *)malloc(buf_len-2); //减去【 和 】
			m_strcpy(soft_array[soft_num-1].name,line_buf,1,buf_len-3);
		}
		else
		{
			int index = find_symbol(line_buf);
			if (index ==-1)
				continue;
			int value_len = buf_len-1-index;
			if(is_equal(line_buf,"host")==TRUE)
			{
				soft_array[soft_num-1].host=(char *)malloc(value_len);
				m_strcpy(soft_array[soft_num-1].host,line_buf,index+1,buf_len-2);
			}
			else if(is_equal(line_buf,"path")==TRUE)
			{
				soft_array[soft_num-1].path=(char *)malloc(value_len);
				m_strcpy(soft_array[soft_num-1].path,line_buf,index+1,buf_len-2);


			}
			else if (is_equal(line_buf,"source")==TRUE)
			{
				soft_array[soft_num-1].source=(char *)malloc(value_len);
				m_strcpy(soft_array[soft_num-1].source,line_buf,index+1,buf_len-2);
			}

		}

	}
	fclose(fp);
}

