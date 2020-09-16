#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <arpa/inet.h>
#include <err.h>
#include <getopt.h>
#include <iostream>
#include <vector>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <netinet/in.h>//?
#include <netinet/ip.h>//?
#include <netinet/ip6.h>
#include <netinet/tcp.h>//?
#include <netinet/udp.h>
#include <ifaddrs.h>
#include <string.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <sys/wait.h>
#include<syslog.h>
#include <netdb.h>

#ifdef __linux__
#include <netinet/ether.h> 
#include <time.h>//?
#include <pcap/pcap.h>
#endif

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256 
#endif

static void print_help();
void exit_program(int err);
void handle_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void send_packet();
void listen_on_if(ifaddrs* interface);
int communicate_to_server(char *send_buffer,int s_buffer_size, char *recv_buffer);
int recv_from_server(int send_sock, char *recv_buffer,struct sockaddr_in6 relay);
int process_packet(char *recv_buffer, struct ia_address *ia_result);
int send_to_client(int client_dh_size,struct dhcpv6_packet *dhcp_header, struct dhcpv6_relay_packet *recv_dhcp);

#define INTERNAL_ERROR 1
#define PCAP_ERROR 2
#define MALLOC_ERROR 3
#define WRONG_SERVER_IP 4
#define FORK_ERROR 5
#define BAD_PACKET 6
#define MISSING_ARGUMENT 10

#define SERVER_PORT 547
#define CLIENT_PORT 546

#define SIZE_ETHERNET 14
#define SIZE_IPV6 40
#define SIZE_UDP 8

// https://github.com/42wim/isc-dhcp/blob/master/includes/dhcp6.h

#define UDPBUFFER 350

struct arguments{
    char server_ip [INET6_ADDRSTRLEN];
    bool logging;
    bool debug;
};

//https://stackoverflow.com/questions/14235208/getifaddrs-to-parse-only-the-ip-from-etherenet-interface-eth-or-wlan-interface
#ifndef IN6_IS_ADDR_GLOBAL
#define IN6_IS_ADDR_GLOBAL(a) \
        ((((__const uint32_t *) (a))[0] & htonl(0x70000000)) \
        == htonl (0x20000000))
#endif /* IS ADDR GLOBAL */

#ifndef IN6_IS_ADDR_ULA
#define IN6_IS_ADDR_ULA(a) \
        ((((__const uint32_t *) (a))[0] & htonl(0xfe000000)) \
        == htonl (0xfc000000))
#endif /* IS ADDR ULA */

