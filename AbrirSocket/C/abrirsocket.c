/*Programa para abir un socket; en este caso un socket tipo RAW (crudo)*/
#include <sys/socket.h> //man socket
#include <netpacket/packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <stdio.h>
#include <arpa/inet.h>//para htons
#include <stdlib.h>
#include <unistd.h>//Para close()

int abrirSocketRaw(void);

int abrirSocketRaw(void){
int packet_socket = 0;
packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); //Regresa un -1 si hubo error.
return packet_socket; 
//AF_PACKET see 7 packet man pages.
}


int main (void){

int ds;

ds = abrirSocketRaw();
if (ds < 0)
{
	perror("No pude abrir el socket");
	exit (1);
}
else
printf("Socket abierto correctamente\n");

close(ds);

return 0;
}
