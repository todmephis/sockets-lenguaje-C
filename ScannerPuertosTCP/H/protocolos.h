#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> 
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/netlink.h>
#include <asm/types.h>
#include <ifaddrs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <linux/rtnetlink.h>
#include <signal.h>
#define TTLPATH "/proc/sys/net/ipv4/ip_default_ttl"
#define ETHTYPE_ARP "\x08\x06"
#define ETHTYPE_IP "\x08\x00"
#define BROADHWADDR "\xff\xff\xff\xff\xff\xff"
#define ICMP_PROT 1
#define UDP_PROT 17
#define TCP_PROT 6
#define IPTOTLEN 48
#define BUFFER_SIZE 4096
#define PACKETTOTLEN 62
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

                        /*PROTOTIPOS*/
void usage (char **);
int hostnameToIP(char *  , char *);
const char * getHostnamefromIP(char *);
int IPstringToArray(char *, unsigned char *);
int abrirSocketRaw(void);
int obtenerDatos(int, struct ifreq *, unsigned char *, unsigned char *, unsigned char *);
int getDefaultInterfaz(char *);
int hostIsInNetwork(unsigned char *, unsigned char *, unsigned char *);
int getGatewayAddr(unsigned char *);
void setEthHeader (unsigned char *, unsigned char *, unsigned char *, unsigned char *, int);
void setARPHeader (unsigned char *, unsigned char *, unsigned char *);
void setIPHeader(unsigned char *, unsigned int, unsigned char *, unsigned char *, int);
void setTCPHeader(unsigned char *, unsigned int, unsigned int, unsigned char *, unsigned char *);
void setUDPHeader(unsigned char *, unsigned int, unsigned int, unsigned char *, unsigned char *);
unsigned short checksum (unsigned char *, int);
unsigned int getTTL(void);
void cpyHwAddrFromTrama(unsigned char *, unsigned char *);
void ARP(unsigned char *, unsigned char *, unsigned char *, unsigned char *, int *);
int sendARP(unsigned char *, unsigned char *, unsigned char *, unsigned char *, int, int *, int);
void rcvARP(unsigned char *, unsigned char *, unsigned char *, int, int *, int, unsigned char *);
int ICMP(unsigned char *, int, int, unsigned char *, unsigned char *, unsigned char *, unsigned char *, int, int, int, int);
int rcvICMP(unsigned char *, int, int, int, unsigned int, unsigned int);
int filterICMPreply(unsigned char *, unsigned int, unsigned int);
int rcvTCP(unsigned char *, int, int, int, unsigned int, unsigned int);
int filterTCPreply(unsigned char *, unsigned int, unsigned int);
void enviarTrama (unsigned char *, int, int, int);
int recibeTrama(int, unsigned char *, int);
int filterARPreply(unsigned char *, int, unsigned char *, unsigned char *, int);
void imprimeTrama (unsigned char *, int);