#include <stdio.h>
#include "match.h"
#include "struct.h"
/* api of read_ini.c*/
extern match_info soft_array[SOFT_MAX_NUM];
extern int soft_num;
void ini_purse();

int main(int argc, char const *argv[])
{
	ini_purse();

	int i;
	for(i=0;i<soft_num;i++)
	{
		printf("%s\n",soft_array[i].name );
		printf("%s\n",soft_array[i].host);
		printf("%s\n",soft_array[i].path);
		printf("%s\n",soft_array[i].source);
	}

	char s[]="GET /hoge HTTP/1.1\r\nHost: example.com\r\nCookie: \r\n\r\n";
	char * source=NULL;
	int re = match(s,sizeof(s)-1,&source);
	printf("!!!%d\n", re);
	printf("%s\n",source );
	return 0;
}