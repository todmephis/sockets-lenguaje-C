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
#define TTLPATH "/proc/sys/net/ipv4/ip_default_ttl"
#define MAXHOP 30
#define ETHTYPE_ARP "\x08\x06"
#define ETHTYPE_ICMP "\x08\x00"
#define BROADHWADDR "\xff\xff\xff\xff\xff\xff"
#define ICMP_PROT 1
#define PINGTOTLEN 60
#define BUFFER_SIZE 4096
#include <lista.h>

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
void setIPHeader(unsigned char *, unsigned int, unsigned char *, unsigned char *, int, unsigned int);
unsigned short checksum (unsigned char *, int);
unsigned int getTTL(void);
void cpyHwAddrFromTrama(unsigned char *, unsigned char *);
void ARP(unsigned char *, unsigned char *, unsigned char *, unsigned char *, int *);
int sendARP(unsigned char *, unsigned char *, unsigned char *, unsigned char *, int, int *, int);
void rcvARP(unsigned char *, unsigned char *, unsigned char *, int, int *, int, unsigned char *);
int ICMP(unsigned char *, int, int, unsigned char *, unsigned char *, int, int, int, int, unsigned int, int, node_t *);
int rcvICMP(unsigned char *, int, int, unsigned short, int, int, int, unsigned int, struct timeval *, node_t *);
int filterICMPreply(unsigned char *, int, unsigned short, int, int, unsigned int, struct timeval *, node_t *);
void enviarTrama (unsigned char *, int, int, int);
int recibeTrama(int, unsigned char *, int);
int filterARPreply(unsigned char *, int, unsigned char *, unsigned char *, int);
void imprimeTrama (unsigned char *, int);

int PING(unsigned char *, int, int, unsigned char *, unsigned char *, int, int, int, int, unsigned int);
int rcvPING(unsigned char *, int, int, unsigned short, int, int, struct timeval *);
int filterPINGreply(unsigned char *, int, unsigned short, int, struct timeval *);