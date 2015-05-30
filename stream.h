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

//���ķ�����
#define DIR_C2S 0x01
#define DIR_S2C 0x02
#define DIR_DOUBLE 0x03


//��ַ���Ͷ���
#define ADDR_TYPE_IPV4			1


//Ӧ�ò㿴��������״̬����
#define OP_STATE_PENDING   0
#define OP_STATE_CLOSE     2
#define OP_STATE_DATA      3

//Ӧ�ò㷵�ؽ������
#define APP_STATE_GIVEME   0x00
#define APP_STATE_DROPME   0x01
#define APP_STATE_FAWPKT   0x00
#define APP_STATE_DROPPKT  0x10

//�������Ͷ���
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
	UCHAR addrtype; //��ַ���� ADDR_TYPE_IPV4
	void *paddr;	//	ǿת��struct tuple4�ṹ��
};


struct streaminfo
{
	struct ipaddr addr;      			//���ӵ�ַ
	struct streaminfo *pfather;		//�������
	UCHAR type;					//������� 
	UCHAR threadnum;	      		//�����߳�ID 
	UCHAR  dir:2;            			  //  ���ķ������0x01 c-->s  0x02  s-->c  0x03 c<-->s
	UCHAR  curdir:2;         			  //  ��ǰ����0x01 c-->s  0x02  s-->c
	UCHAR  dirreverse:4;	 		   //������� 
	UCHAR  opstate;				//��ǰ��������״̬
	UCHAR  pktstate;				//�������
	UCHAR  routedir;	    			 //G�豸�����з���
	UCHAR  __pad__[10];		//�ֽڲ���
   	void    *pdetail;			//TCP��UDP������ϸ��Ϣ
 };




struct tcpdetail
{

	void  *pdata;		      //���ݣ�TCP�ĸ���
	UINT32 datalen;		//���ݳ��ȣ�TCP�ĸ��س���
	UINT32 lostlen;		//�������ݳ���
	UINT32 totallost;
	UINT32 pad;

	void* pclient;  	//��������ģ���һ�汾��ɾ����������ʹ��
	void* pserver;  	//��������ģ���һ�汾��ɾ����������ʹ��
	int serverpkt;	//c2s�����ݰ���
	int clientpkt;		//s2c�����ݰ���
	int servercount;	//c2s�����ݳ���
	int clientcount;	//s2c�����ݳ���
	unsigned long creattime; 		//���Ӵ���ʱ��
	unsigned long lastmtime;		//����һ�����ݰ�����ʱ��

};
struct udpdetail
{

   void *pdata;		      //UDP������������
   UINT32 datalen;			  //UDP�����������ݳ���

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

