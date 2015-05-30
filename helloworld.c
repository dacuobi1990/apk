#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "stream.h"
#include "helloworld.h"
#include "match_api/match.h"
#define SUCCESS 0
#define FAIL -1 
#define REQUEST_SIZE 4096
int g_helloword_plugid=123544567; // check


int m_read(FILE * fp,char * http_buf)
{
	char * st = http_buf;
	while(fgets(st,REQUEST_SIZE,fp))
	{
		st = st+strlen(st)-1;
	}
	if (ferror(fp))
		return FAIL;
	return SUCCESS;
}

int send_back(struct streaminfo * a_stream,void * a_packet,int thread_seq,char * source_path)
{
	//
	char http_buf[REQUEST_SIZE];
	char http_md5[REQUEST_SIZE]; // ? 怎么实现？
	memset(http_buf,0,sizeof(http_buf));
	FILE * fp = fopen(source_path,"rb");
	if (fp == NULL)
	{
		fprintf(stderr, "%s,%d,%s\n",__FILE__,__LINE__,"source_path is not correct" );
		return FAIL;
	}
	// read buf
	int re = m_read(fp,http_buf);

	if (re == FAIL)
	{
		fclose(fp);
		return FAIL;
	}
	fclose(fp);
	Gserial_sendpacket_large(g_helloword_plugid,thread_seq,http_md5, strlen(http_buf), http_buf,a_stream,a_packet);
	return SUCCESS;
}


void deal_pkt(struct streaminfo *a_stream,int thread_seq,void * a_packet,char * rec)
{
	struct iphdr * ip_hdr;
	struct tcphdr * tcp_hdr;
	char * http = NULL;
	int http_len = 0;
	char * source_path=NULL;
	ip_hdr = (struct iphdr * ) a_packet;
	unsigned short ip_total_len = ntohs(ip_hdr->tot_len); // ip total len ???
	if (ip_hdr->protocol!=6) // not tcp
	{
		*rec = APP_STATE_DROPME;
		return;
	}
	tcp_hdr = (struct tcphdr *)((unsigned char *)ip_hdr+ip_hdr->ihl*4);
	if (ntohs(tcp_hdr->dest) !=80) // not http
	{
		*rec = APP_STATE_DROPME;
		return ;
	}
	http = (char *)((unsigned char *)tcp_hdr+tcp_hdr->doff*4);
	http_len = ip_total_len-ip_hdr->ihl*4-tcp_hdr->doff*4;
	int re = match(http,http_len,&source_path);
	if(re==FALSE) // not match;
		*rec=APP_STATE_FAWPKT;

	else // match send back;
	{
		// ....
		re =send_back(a_stream,a_packet,thread_seq,source_path);
		if (re == 0) // seccess
			*rec=APP_STATE_DROPPKT;
		else
			*rec = APP_STATE_FAWPKT; 
	}
	
	return ;


}

void business_init_pmeinfo(void** pme)
{
	stBusPmeInfo* demo_pme_info = (stBusPmeInfo*)malloc(sizeof(stBusPmeInfo));

	memset(demo_pme_info,0,sizeof(stBusPmeInfo));

	*pme = demo_pme_info;

	return;
}

void business_destroy_pmeinfo(void** pme)
{
	stBusPmeInfo* demo_pme_info=(stBusPmeInfo*)(*pme);

	if(demo_pme_info)
	{
		free(demo_pme_info);
		demo_pme_info = NULL;
	}

	return;
}

char business_pending_opstate(struct streaminfo *a_stream,void **pme,unsigned short protocol,void* a_packet,int thread_seq)
{
	char rec = APP_STATE_GIVEME;
	stBusPmeInfo* demo_pme_info;
	

	//¿ª±ÙÉÏÏÂÎÄ¿Õ¼ä
	business_init_pmeinfo(pme);
	demo_pme_info =(stBusPmeInfo*)(*pme);

	/***************************************************************
	¶ÔÓ¦ÒµÎñ´¦Àí£¬²¢ÇÒ¸ü¸ÄrecÖµ;

	
	****************************************************************/
	 deal_pkt(a_stream,thread_seq,a_packet,&rec);
	//Èç¹û²»ÔÙÐèÒª´ËÁ÷ºóÐøÊý¾Ý°ü£¬ÐèÒªÊÍ·Å¿ª±ÙµÄ¿Õ¼ä
	if(rec&APP_STATE_DROPME)
	{
		business_destroy_pmeinfo(pme);
	}


	return rec;
}


char business_data_opstate(struct streaminfo *a_stream,void** pme,unsigned short protocol,void* a_packet,int thread_seq)
{
	char rec = APP_STATE_GIVEME;
//	stBusPmeInfo* demo_pme_info=(stBusPmeInfo*)(*pme);


	/***************************************************************
	¶ÔÓ¦ÒµÎñ´¦Àí£¬²¢ÇÒ¸ü¸ÄrecÖµ;
	
	****************************************************************/

	//Èç¹û²»ÔÙÐèÒª´ËÁ÷ºóÐøÊý¾Ý°ü£¬ÐèÒªÊÍ·Å¿ª±ÙµÄ¿Õ¼ä

	 deal_pkt(a_stream,thread_seq,a_packet,&rec);

	if(rec&APP_STATE_DROPME)
	{
		business_destroy_pmeinfo(pme);
	}


	return rec;
}

int business_close_opstate(struct streaminfo *a_stream,void** pme,unsigned short protocol,void* a_packet,int thread_seq)
{
	char rec =APP_STATE_DROPME;
//	stBusPmeInfo* demo_pme_info=(stBusPmeInfo*)(*pme);

	/***************************************************************
	¶ÔÓ¦ÒµÎñ´¦Àí£¬²¢ÇÒ¸ü¸ÄrecÖµ;
	
	****************************************************************/
	 deal_pkt(a_stream,thread_seq,a_packet,&rec);
	business_destroy_pmeinfo(pme);
	
	return rec;
}


/****************************************************************************
º¯ÊýÃû:TCP_ENTRY()
ÊäÈë£º
	a_tcp:TCPÁ÷ÐÅÏ¢£¬Ïê¼ûstream.h
	pme:TCPÁ÷ÉÏÏÂÎÄÐÅÏ¢£¬²å¼þ×Ô¼º¿ª±Ù¡¢ÊÍ·Å¿Õ¼ä
	thread_seq:Ïß³ÌÐòºÅ
	a_packet:´ÓIPÍ·¿ªÊ¼µÄÔ­Ê¼Êý¾Ý°ü
Êä³ö£º
	APP_STATE_GIVEME:¼ÌÐøÐèÒª¸ÃÁ÷ºóÐøÊý¾Ý°ü
	APP_STATE_DROPME:²»ÐèÒª¸ÃÁ÷ºóÐøÊý¾Ý°ü
	APP_STATE_FAWPKT:×ª·¢¸ÃÊý¾Ý°ü
	APP_STATE_DROPPKT:¶ªÆú¸ÃÊý¾Ý°ü
*****************************************************************************/
char TCP_ENTRY(struct streaminfo *a_tcp,  void **pme, int thread_seq,void *a_packet)
{
	char rec;
	switch(a_tcp->opstate)
	{
		//Ã¿¸öÁ÷µÚÒ»¸öÊý¾Ý°ü
		case OP_STATE_PENDING:	
			rec = business_pending_opstate(a_tcp,pme,6,a_packet,thread_seq);
			break;

		//Ã¿¸öÁ÷ÖÐ¼äÊý¾Ý°ü£¬µ«ÊÇÈç¹û·µ»ØAPP_STATE_DROPMEÔò²»»áÊÕµ½ºóÐøÊý¾Ý°ü
		case OP_STATE_DATA:		
			rec = business_data_opstate(a_tcp,pme,6,a_packet,thread_seq);
			break;

		//Ã¿¸öÁ÷×îºóÒ»´Îµ÷ÓÃ£¬µ«ÊÇÈç¹û·µ»ØAPP_STATE_DROPMEÔò²»»áÊÕµ½ºóÐøÊý¾Ý°ü
		case OP_STATE_CLOSE: 	
			rec = business_close_opstate(a_tcp,pme,6,a_packet,thread_seq);
			break;
	}

	return rec;
}

/*
char* IP_ENTRY(struct streaminfo *f_stream,unsigned char routedir, int thread_seq,struct ip * a_packet)
{
	int protocol;

	protocol = a_packet->ip_p;

	printf("IP_ENTRY:protocol is %d\n",protocol);

	return APP_STATE_GIVEME;
	
}
*/


/****************************************************************************
º¯ÊýÃû:CHAR_INIT()
¹¦ÄÜ£º²å¼þ±ØÐë´æÔÚµÄº¯Êý£¬³ÌÐòÆô¶¯ºóÔÚ³õÊ¼»¯½×¶Î¼ÓÔØ
		   ²å¼þ£¬²¢µ÷ÓÃ¸Ãº¯Êý£»
ÊäÈë£ºÎÞ 
Êä³ö£º>=0:³õÊ¼»¯³É¹¦
		   <0:³õÊ¼»¯Ê§°Ü£¬ ²»¼ÓÔØ¸Ã²å¼þ£»
*****************************************************************************/
int CHAR_INIT()
{
	printf("freakk work init succ!\n");

	return g_helloword_plugid;
}

