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

					/*PROTOTIPOS*/

//VALOR DE RETORNO: int >> descriptor del socket abierto
int abrirSocketRaw(void);

/*int >> Descriptor del socket abierto.
struct ifreq * >> Apuntador de la estructura ifreq para obtener los datos de la interfaz de red.
unisgned char * >> Apuntador para almacenar la dirección MAC de mi interfaz de red.
unisgned char * >> Apuntador para almacenar la dirección lógica de mi interfaz de red.
unsigned char * >> Apuntador para almacenar la dirección de broadcast.
unsigned char * >> Apuntador para almacenar la máscara de red.
VALOR DE RETORNO: int >> Índice de mi interfaz de red (usado posteriormente...)*/
int obtenerDatos(int, struct ifreq *, unsigned char *, unsigned char *, unsigned char *, unsigned char *);

					/*FUNCIONES*/

int abrirSocketRaw(void){
int packet_socket = 0;
packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
if(packet_socket == -1){
	perror("Error al abrir el socket");
	exit (EXIT_FAILURE);
}
return packet_socket; 
}

int obtenerDatos(int ds, struct ifreq * ptrInterfaz, unsigned char * ownMAC, unsigned char * ownIP, unsigned char * ownBroadcast, unsigned char * ownNetMask){
int index=0;

printf("Interfaz de red >> ");
scanf("%s", ptrInterfaz->ifr_name);
//ioctl regresa un -1 en caso de errores.
//Comienzo a obtener los datos de la interfaz ingresada
//Índice
if(ioctl (ds, SIOCGIFINDEX, ptrInterfaz) == -1)
	{
	perror("Error al obtener el índice");
	exit (EXIT_FAILURE);
	}
else
	{
	index = ptrInterfaz->ifr_ifindex;
    printf ("Índice >> %d\n", index);
	}
//Dirección MAC
if (ioctl (ds, SIOCGIFHWADDR, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la dirección MAC");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownMAC, ptrInterfaz->ifr_hwaddr.sa_data, 6);
      printf ("Dirección MAC >> ");
      fprintf(stdout, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", ownMAC[0], ownMAC[1], ownMAC[2], ownMAC[3], ownMAC[4], ownMAC[5]);
    }	
//Dirección IP
if (ioctl (ds, SIOCGIFADDR, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la dirección IP");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownIP, ptrInterfaz->ifr_addr.sa_data + 2, 4);
      printf ("IPv4 >> ");
      fprintf(stdout, "%d.%d.%d.%d\n", ownIP[0], ownIP[1], ownIP[2], ownIP[3]);
    }
//Dirección de Broadcast
if (ioctl (ds, SIOCGIFBRDADDR, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la broadcast");
      exit (EXIT_FAILURE);
    }
else
    {
     memcpy (ownBroadcast, ptrInterfaz->ifr_broadaddr.sa_data + 2, 4);
     printf ("Dirección de broadcast >> ");
     fprintf(stdout, "%d.%d.%d.%d\n", ownBroadcast[0], ownBroadcast[1], ownBroadcast[2], ownBroadcast[3]);
    }
//Máscara de red
if (ioctl (ds, SIOCGIFNETMASK, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la mascara");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownNetMask, ptrInterfaz->ifr_netmask.sa_data + 2, 4);
      printf ("Máscara de subred >> ");
      fprintf(stdout, "%d.%d.%d.%d", ownNetMask[0], ownNetMask[1], ownNetMask[2], ownNetMask[3]);
    }
  printf ("\n");

return index;
}
						//MAIN
int main (void){
int ds, indice = 0;
unsigned char ownMAC[6], ownIP[4], ownBroadcast[4], ownNetMask[4];
struct ifreq Interfaz;
struct ifreq * ptrInterfaz;
ptrInterfaz = &Interfaz;

ds=abrirSocketRaw();
indice=obtenerDatos(ds, ptrInterfaz, ownMAC, ownIP, ownBroadcast, ownNetMask);
close(ds);
return 0;
}