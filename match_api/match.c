#include <stdio.h>
#include "struct.h"
#include "match.h"
#include <string.h>
#include "pico/picohttpparser.h"

/* api of read_ini.c*/
extern match_info soft_array[SOFT_MAX_NUM];
extern int soft_num;
void ini_purse();


void match_init()
{
	ini_purse(); // read the info from config,store in soft_array 
}


static int check_ini(const char * host,int host_len,const char * path,int path_len)
{
	int i;
	for(i=0;i<soft_num;i++)
	{
		if(memcmp(host,soft_array[i].host,host_len)==0 && memcmp(path,soft_array[i].path,path_len)==0)
			return i;
	}
	return -1;
}


int match(const char * pload,int len,char ** source_path)
{
	const char* method;
  	size_t method_len;
  	const char* path;
  	size_t path_len;
  	int minor_version;
  	struct phr_header headers[HEADER_NUM];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);

    int re =phr_parse_request(pload,len, &method, &method_len, &path,&path_len, &minor_version, headers,&num_headers,0);  
    printf("~~~~%d~~~\n",re );
    if(re<=0)
    	return FALSE;

    // find_host
    int i;
    for(i=0;i<num_headers;i++)
    {
    	if (memcmp(headers[i].name,"Host",headers[i].name_len)==0)
    	{
    		int index = check_ini(headers[i].value,headers[i].value_len,path,path_len);
    		if(index!=-1)
    		{
    			*source_path = soft_array[index].source;
    			return TRUE;
    		}

    	}
    }
    return FALSE;


}
