#ifndef _APP_STREAM_H_
#define _APP_STREAM_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char      UINT8;
typedef unsigned long long UINT64;
typedef unsigned int       UINT32;
typedef unsigned short     UINT16;
typedef unsigned char      UCHAR;

//流的方向定义
#define DIR_C2S 0x01
#define DIR_S2C 0x02
#define DIR_DOUBLE 0x03


//地址类型定义
#define ADDR_TYPE_IPV4			1


//应用层看到的链接状态定义
#define OP_STATE_PENDING   0
#define OP_STATE_CLOSE     2
#define OP_STATE_DATA      3

//应用层返回结果定义
#define APP_STATE_GIVEME   0x00
#define APP_STATE_DROPME   0x01
#define APP_STATE_FAWPKT   0x00
#define APP_STATE_DROPPKT  0x10

//流的类型定义
#define STREAM_TYPE_TCP				1
#define STREAM_TYPE_UDP				2

struct tuple4 {
   unsigned int saddr;
   unsigned int daddr;
   unsigned short source;
   unsigned short dest;
 };

struct ipaddr
{
	UCHAR addrtype; //地址类型 ADDR_TYPE_IPV4
	void *paddr;	//	强转成struct tuple4结构体
};


struct streaminfo
{
	struct ipaddr addr;      			//链接地址
	struct streaminfo *pfather;		//插件忽略
	UCHAR type;					//插件忽略 
	UCHAR threadnum;	      		//所属线程ID 
	UCHAR  dir:2;            			  //  流的方向情况0x01 c-->s  0x02  s-->c  0x03 c<-->s
	UCHAR  curdir:2;         			  //  当前来包0x01 c-->s  0x02  s-->c
	UCHAR  dirreverse:4;	 		   //插件忽略 
	UCHAR  opstate;				//当前链接所处状态
	UCHAR  pktstate;				//插件忽略
	UCHAR  routedir;	    			 //G设备上下行方向
	UCHAR  __pad__[10];		//字节补齐
   	void    *pdetail;			//TCP或UDP链接详细信息
 };




struct tcpdetail
{

	void  *pdata;		      //数据，TCP的负载
	UINT32 datalen;		//数据长度，TCP的负载长度
	UINT32 lostlen;		//丢包数据长度
	UINT32 totallost;
	UINT32 pad;

	void* pclient;  	//插件不关心，下一版本将删除，不建议使用
	void* pserver;  	//插件不关心，下一版本将删除，不建议使用
	int serverpkt;	//c2s的数据包数
	int clientpkt;		//s2c的数据包数
	int servercount;	//c2s的数据长度
	int clientcount;	//s2c的数据长度
	unsigned long creattime; 		//链接创建时间
	unsigned long lastmtime;		//最新一个数据包到达时间

};
struct udpdetail
{

   void *pdata;		      //UDP负载内容数据
   UINT32 datalen;			  //UDP负载内容数据长度

  int serverpkt;
  int clientpkt;
  int servercount;
  int clientcount;
  unsigned long creattime; 
  unsigned long lastmtime;
};

/*
char IP_ENTRY( struct streaminfo *f_stream,unsigned char routedir,int thread_seq,struct ip * a_packet);
char TCP_ENTRY(struct streaminfo *a_tcp,  void **pme, int thread_seq,void *a_packet);
char UDP_ENTRY(struct streaminfo *a_udp,  void **pme, int thread_seq,void *a_packet);
*/

#endif

