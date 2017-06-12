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
#define TRAMA_LEN 60

					/*PROTOTIPOS*/

//VALOR DE RETORNO: int >> descriptor del socket abierto
int abrirSocketRaw(void);

/* int >> Descriptor del socket abierto.
struct ifreq * >> Apuntador de la estructura ifreq para obtener los datos de la interfaz de red.
unisgned char * >> Apuntador para almacenar la dirección MAC de mi interfaz de red.
unisgned char * >> Apuntador para almacenar la dirección lógica de mi interfaz de red.
unsigned char * >> Apuntador para almacenar la dirección de broadcast.
unsigned char * >> Apuntador para almacenar la máscara de red.
VALOR DE RETORNO: int >> Índice de mi interfaz de red (usado posteriormente...). */
int obtenerDatos(int, struct ifreq *, unsigned char *, unsigned char *, unsigned char *, unsigned char *);

/* unisigned char * >> Apuntador a la trama a imprimir.
int >> tamaño de la trama a imprimir. */
void imprimeTrama (unsigned char *, int);

/*int >> Descriptor del socket abierto.
unsigned char * >> Apuntador para almacenar la trama que recibimos.*/
void recibeTrama(int, unsigned char *);

					/*FUNCIONES*/

int 
abrirSocketRaw(void)
{
int packet_socket = 0;
packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
if(packet_socket == -1){
	perror("Error al abrir el socket");
	exit (EXIT_FAILURE);
}
return packet_socket; 
}

int 
obtenerDatos(int ds, struct ifreq * ptrInterfaz, unsigned char * ownMAC, unsigned char * ownIP, unsigned char * ownBroadcast, unsigned char * ownNetMask)
{
int index=0;
printf("Interfaz de red >> ");
scanf("%s", ptrInterfaz->ifr_name);
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

void
imprimeTrama (unsigned char *trama, int tramalen)
{
  int i;
  for (i = 0; i < tramalen; i++)
    {
        if (i % 16 == 0)
        printf ("\n");
      printf ("%.2x ", *(trama + i));

    }
  printf ("\n");
}	

void recibeTrama(int ds, unsigned char * trama){
int tam=0;
int lenght=0;
//unsigned char ethtype[2]={0xae,0xae};//Ethertype personalizado para identificar el paquete
while(1){

	tam=recvfrom(ds, trama, 1514, 0, NULL, 0);
 	lenght = trama[13] | trama[12] << 8;
	if(tam == -1)
		perror("Error al recibir la trama\n");
	if(lenght <= 1500)//FILTRO >> Recibo tramas que en el campo de longitud sea menor o igual a 1500.
		{
      printf("\nTrama IEEE 802.3");
      fprintf(stdout, "\nMAC Destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t", trama[0], trama[1], trama[2], trama[3], trama[4], trama[5]);
      fprintf(stdout, "MAC Origen: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", trama[6], trama[7], trama[8], trama[9], trama[10], trama[11]);
      printf("Longitud: %d\n", lenght);
		}	

}

}

int main (void){
int ds, indice = 0;
unsigned char ownMAC[6], ownIP[4], ownBroadcast[4], ownNetMask[4];
struct ifreq Interfaz;
struct ifreq * ptrInterfaz;
ptrInterfaz = &Interfaz;
unsigned char trama_recibir[1514];

ds=abrirSocketRaw();
indice=obtenerDatos(ds, ptrInterfaz, ownMAC, ownIP, ownBroadcast, ownNetMask);
recibeTrama(ds, trama_recibir);
close (ds);

return 0;

}