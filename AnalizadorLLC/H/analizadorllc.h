#include <sys/socket.h> 
#include <netpacket/packet.h>
#include <net/ethernet.h> 
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <linux/netlink.h>
#include <asm/types.h>
#include <sys/types.h>
#include <ifaddrs.h>


					/*PROTOTIPOS*/
int abrirSocketRaw(void);
int obtenerDatos(int, struct ifreq *, unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void imprimeTrama (unsigned char *, int);
void recibeTrama(int, unsigned char *);
void leerArchivoLLC(FILE *);
void analizaLLC(unsigned char *);
void usage (char **);
int getDefaultInterfaz(char *);