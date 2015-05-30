#ifndef _DEMO_BUESSNES_H
#define _DEMO_BUESSNES_H

typedef struct __ipinfo_ip6_hdr
{
    unsigned char ip6_flags[4]; 	/* version, traffic-class, flow-label */
    unsigned short ip6_payload_len;  	/* payload length, not contain header */
    unsigned char ip6_nxt_hdr;      /* next header, same as protocol in IPv4 */
    unsigned char ip6_hop;          /* hop limit, same as TTL in IPv4 */
    struct in6_addr ip6_src;		/* source address */
    struct in6_addr ip6_dst; 		/* dest address */
}ipinfo_header_ipv6;


typedef struct _demo_business_pme_info
{
	int a;
	int b;
}stBusPmeInfo;

void Gserial_sendpacket_large(int plug_id,int thread_index,char* cont_md5, int cont_len, char* cont_buf,struct streaminfo* a_tcp,void *a_packet);


#endif
