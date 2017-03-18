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

/* unsigned char * >> Apuntador a la trama para ser enviada.
int >> tamaño de la trama.
int >> descriptor del socket abierto.
int >> índice de la interfáz de red. */
void enviarTrama (unsigned char *trama_enviar, int tramalen, int ds, int index);

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

void
enviarTrama (unsigned char *trama_enviar, int tramalen, int ds, int index)
{
  int tam;
  struct sockaddr_ll interfaz;
  memset (&interfaz, 0x00, sizeof (interfaz));  //seteamos la interfaz a cero
  interfaz.sll_family = AF_PACKET;
  interfaz.sll_protocol = htons (ETH_P_ALL);
  interfaz.sll_ifindex = index;
  tam =
    sendto (ds, trama_enviar, tramalen, 0, (struct sockaddr *) &interfaz,
      sizeof (interfaz));
  if (tam == -1)
    {
      perror ("No pude enviar la trama");
      exit(EXIT_FAILURE);
    }
  else
    {
      printf ("Trama satisfactoriamente enviada\n");
    }

}



						//MAIN
int 
main (void)
{
int ds, indice = 0;
unsigned char ownMAC[6], ownIP[4], ownBroadcast[4], ownNetMask[4];
struct ifreq Interfaz;
struct ifreq * ptrInterfaz;
ptrInterfaz = &Interfaz;

unsigned char mi_trama_para_enviar[1514];
unsigned char mensaje_para_la_trama[10]={'S','o','y', ' ', 'I', 'v', 'a', 'n'};
unsigned char MACbroadcast[6]={0xff,0xff,0xff,0xff,0xff,0xff};//MAC DE BROADCAST ff:ff:ff:ff:ff:ff
unsigned char ethtype[2]={0xae,0xae};//Ethertype personalizado para identificar el paquete
unsigned char cMAC[6]={0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};

ds=abrirSocketRaw();
indice=obtenerDatos(ds, ptrInterfaz, ownMAC, ownIP, ownBroadcast, ownNetMask);
//Estructuramos la trama con los datos deseados
memset(mi_trama_para_enviar, 0x00, sizeof(mi_trama_para_enviar)/sizeof(mi_trama_para_enviar[0]));
memcpy(mi_trama_para_enviar+0, MACbroadcast, 6);//Del valor 0 hasta el 5 son los 6 bytes de la dirección física destino.
//memcpy(mi_trama_para_enviar+6, ownMAC, 6);//Del 6 al 11 son los 6 bytes de mi dirección MAC.
memcpy(mi_trama_para_enviar+6,cMAC,6);//AÑADE UNA MAC INVENTADA A LA TRAMA EN LUGAR DE LA ORIGINAL.
memcpy(mi_trama_para_enviar+12, ethtype, 2);//Del 12 al 14 son 2 bytes de ethertype.
memcpy(mi_trama_para_enviar+14, mensaje_para_la_trama, strlen(mensaje_para_la_trama));//Mensaje para enviar en la trama.
printf("Trama a enviar:");
imprimeTrama(mi_trama_para_enviar, TRAMA_LEN);
enviarTrama(mi_trama_para_enviar, TRAMA_LEN, ds, indice);
close(ds);
return 0;
}