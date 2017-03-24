#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <stdlib.h>
#include <linux/netlink.h>
#include <asm/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
//MYSQL DEV
#include <my_global.h>
#include <mysql.h>
#include <termios.h>
//Colores para imprimir en stdout.
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define BLU   "\x1B[34m"
#define BLU   "\x1B[34m"
#define TRAMA_LEN 61
#define IP_LEN 17

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

/*unsigned char * >> Apuntador para almacenar la trama que recibimos.
int >> Descriptor del socket abierto.
int >> Indice de la interfaz
unsigned char * >> Apuntador a la trama a enviar.
MYSQL * >> Apuntador a la conexión MySQL.*/
void recibeTrama(unsigned char *, int, int, unsigned char *, MYSQL *);

/* unsigned char * >> Apuntador a la trama para ser enviada.
int >> tamaño de la trama.
int >> descriptor del socket abierto.
int >> índice de la interfáz de red. */
void enviarTrama (unsigned char *, int, int, int);

/*unsigned char * >> Apuntador a la trama para ser enviada.
unsigned char * >> Apuntador a la MAC destino.
unsigned char * >> Apuntador a la MAC origen.
unsigned char * >> Apuntador al EtherType (para arp es 0806).
unsigned char * >> Apuntador al Hardware type y Protocol type.
unsigned char * >> Apuntador al Hardware address lenght y Protocol address lenght.
unsigned char * >> Apuntador al Operation Code.
unsigend char * >> Apuntador al Source Hardware Address. (MAC origen).
unsigned char * >> Apuntador al Source Protocol Address. (IP origen).
unsigned char * >> Apuntador al Target Hardware Address. (MAC destino).
unsigned char * >> Apuntador al Target Protocol Address. (IP destino).*/
void
estructuraTrama (unsigned char *, unsigned char *,
     unsigned char *, unsigned char *,
     unsigned char *, unsigned char *, unsigned char *,
     unsigned char *, unsigned char *,
     unsigned char *, unsigned char *);


//MYSQL * >> Apuntador a la conexión de MySQL.
void finish_with_error (MYSQL *);

//MYSQL * >> Apuntador a la conexión de MySQL.
void conectaDB (MYSQL *);

//char * >> Apuntador a la contraseña(se captura invisible en stdin).
void getPassword(char *);

void flush_in(void); // >> limpia el buffer de stdin.

/*Convierte la dirección MAC recibida de la consulta MySQL (en forma de cadena)
a un unsigned char array de 6 bytes para poder compara con la trama recibida
El primer argumento es donde se almacena la MAC en 6 bytes y la segunda es la 
MAC en forma de cadena recibida de la consulta.
*/
void
convertirMACa6bytes (unsigned char *MAC_bytes, MYSQL_ROW * MAC_string);


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
printf("Interfaz de red >> "YEL);
scanf("%s", ptrInterfaz->ifr_name);
printf(RESET);
if(ioctl (ds, SIOCGIFINDEX, ptrInterfaz) == -1)
	{
	perror("Error al obtener el índice");
	exit (EXIT_FAILURE);
	}
else
	{
	index = ptrInterfaz->ifr_ifindex;
    printf ("Índice >> "YEL"%d\n", index);
    printf(RESET);
	}
if (ioctl (ds, SIOCGIFHWADDR, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la dirección MAC");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownMAC, ptrInterfaz->ifr_hwaddr.sa_data, 6);
     //printf ("Dirección MAC >> "YEL);
     //fprintf(stdout, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", ownMAC[0], ownMAC[1], ownMAC[2], ownMAC[3], ownMAC[4], ownMAC[5]);
     //printf(RESET);
    }	
if (ioctl (ds, SIOCGIFADDR, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la dirección IP");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownIP, ptrInterfaz->ifr_addr.sa_data + 2, 4);
      //printf ("IPv4 >> "YEL);
      //fprintf(stdout, "%d.%d.%d.%d\n", ownIP[0], ownIP[1], ownIP[2], ownIP[3]);
      //printf(RESET);
    }
if (ioctl (ds, SIOCGIFBRDADDR, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la broadcast");
      exit (EXIT_FAILURE);
    }
else
    {
     memcpy (ownBroadcast, ptrInterfaz->ifr_broadaddr.sa_data + 2, 4);
     //printf ("Dirección de broadcast >> "YEL);
     //fprintf(stdout, "%d.%d.%d.%d\n", ownBroadcast[0], ownBroadcast[1], ownBroadcast[2], ownBroadcast[3]);
     //printf(RESET);
    }
if (ioctl (ds, SIOCGIFNETMASK, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la mascara");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownNetMask, ptrInterfaz->ifr_netmask.sa_data + 2, 4);
      //printf ("Máscara de subred >> "YEL);
      //fprintf(stdout, "%d.%d.%d.%d", ownNetMask[0], ownNetMask[1], ownNetMask[2], ownNetMask[3]);
      //printf(RESET);
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

void recibeTrama(unsigned char * trama_recibo, int ds, int indice, unsigned char *trama_envio, MYSQL * con)
{
int tam = 0;
  /*TRAMA ARP */
  unsigned char MAC_DD[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  unsigned char ZERO_IP[4] = {0,0,0,0};
  unsigned char ethtype[2] = { 0x08, 0x06 };
  unsigned char HW_PR[4] = { 0x00, 0x01, 0x08, 0x00 };
  unsigned char HS_PS[2] = { 0x06, 0x04 };
  unsigned char OPCODE_REPLY[2]={0x00,0x02};
  unsigned char OPCODE_REQ[2]={0x00,0x01};
  unsigned char THA_NULL[6]={0x00,0x00,0x00,0x00,0x00,0x00};
  /*Variables para los datos del atacante (Target Protocol Address and Source Procolo Address)
     IP en 4 bytes y en cadena */
  unsigned char IP_TPAi[4];
  unsigned char IP_SPAi[4];
  unsigned char IP_TPAiString[17];
  unsigned char IP_SPAiString[17];
  /*Variables para la MAC del ifractor*/
  unsigned char MAC_THAi[6];
  /*Variables para almacenar la MAC sin puntos */
  unsigned char MACd[6];
  /*Variable para almacenar la Source Hardware address*/
  unsigned char MAC_SHA[6];
  /*Variables para hacer la consulta en MySQL */
  char query[150];
  MYSQL_ROW row;
  MYSQL_RES *result;
  

  memset (&query, 0x00, 150);
  memset (&IP_TPAi, 0x00, 4);
  memset (&IP_SPAi, 0x00, 4);
  memset (&IP_TPAiString, 0x00, 4);
  memset (&IP_SPAiString, 0x00, 4);
  memset (&MACd, 0x00, 4);
  memset(&MAC_THAi, 0x00, 6);
  

  while (1)
  {
      tam = recvfrom (ds, trama_recibo, 1514, 0, NULL, 0);

      if (!memcmp (trama_recibo + 0, MAC_DD, 6) &&//Si la trama recibida tiene en la Direccion Destino la MAC de broadcast
	  !memcmp (trama_recibo + 12, ethtype, 2) && // y si tiene el Ethertype 0806
	  !memcmp (trama_recibo + 14, HW_PR, 4) &&// y si tiene el HW 0001 y PR 0800
	  !memcmp (trama_recibo + 18, HS_PS, 2) &&// y si tiene HS 06 y PS 04
	  !memcmp (trama_recibo + 20, OPCODE_REQ, 2) &&// y si tiene el OpCode 0001 (ARP request) Entonces es una peticion ARP
    !memcmp (trama_recibo + 32, THA_NULL, 6))// y si THA es 00:00:00:00...
	   {
    	  if (!memcmp (trama_recibo + 28, ZERO_IP, 4)//Si la trama tiene 0.0.0.0 en SPA o el SPA y TPA son iguales 
    	      || !memcmp (trama_recibo + 28, trama_recibo + 38, 4))//Entonces es ARPgrat
    	    {
              if(!memcmp(trama_recibo + 28, ZERO_IP, 4)){//Si el SPA es 0.0.0.0 entonces
                memcpy (IP_SPAi, trama_recibo + 38, 4);//Copio la TPA de la trama en lugar del SPA
              }
              else{
                memcpy (IP_SPAi, trama_recibo + 28, 4);//Copio la SPA de la trama (del atacante)
              }
            memcpy(MAC_THAi, trama_recibo+6, 6);//MAC del ifractor
    	      memcpy (IP_TPAi, trama_recibo + 38, 4);//Copio la TPA de la trama (del atacante)
    	      sprintf (IP_SPAiString, "%d.%d.%d.%d", IP_SPAi[0], IP_SPAi[1],
    		       IP_SPAi[2], IP_SPAi[3]);//Las convierto a cadenas a.b.c.d
    	      sprintf (IP_TPAiString, "%d.%d.%d.%d", IP_TPAi[0], IP_TPAi[1],
    		       IP_TPAi[2], IP_TPAi[3]);
    	      sprintf (query,
    		       "SELECT dir_mac FROM direcciones WHERE dir_ip = '%s' OR dir_ip = '%s'",
    		       IP_SPAiString, IP_TPAiString);//Armo la cadena de la consulta con las 2 IP como cadenas
    	      mysql_query (con, query);//Ejecuto la consulta
    	      result = mysql_store_result (con);//Guardo el resultado en un apuntador
    	       if (mysql_num_rows (result) == 0)//Si la consulta regresa empy set o sea 0 rows entonces no existe en a BDD
    		      {
    		         break;
    		      }
        	    else//De lo contrario...
        		  {
            		  row = mysql_fetch_row (result);//Guarda la columna devuelta (Siempre solo debe ser una)
                  
            		  convertirMACa6bytes (MACd, &row);//Convierto la cadena devuelta a unsigned char de 6  bytes
            		  memcpy(MAC_SHA, trama_recibo+22, 6);
            	  printf(RED"\n================================================="RESET);
                  printf(CYN"\n    ARP Gratuitous \"entrante\"(Petición)\n");
                  imprimeTrama(trama_recibo, tam);
                  printf(RESET MAG"   MAC ORIGEN\t\tIP SOLICIDATA\tMAC ASOCIADA EN LA BDD\n"RESET);
                  printf(BLU);
                  fprintf(stdout, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", MAC_SHA[0], MAC_SHA[1], MAC_SHA[2], MAC_SHA[3], MAC_SHA[4], MAC_SHA[5]);
                  fprintf (stdout, "\t%d.%d.%d.%d", IP_TPAi[0], IP_TPAi[1],IP_TPAi[2], IP_TPAi[3]);
                  printf ("\t  %s\n", *row);//Imprimo la MAC ASOCIADA como cadena
                  printf(RESET);
            		  if (!memcmp (MACd, trama_recibo + 22, 6))//Si la mac en la base de datos es igual a la que 
            		    {//viene en la trama entonces todo está en orden
            		      printf(GRN"\nDirección MAC Origen y la Dirección IP solicitada coindicen\ncon el registro en la BDD.....(OK)"RESET"");
            		      break;
            		    }
            		  else//De lo contrario 
            		    {
            		      printf(RED"\nDirección MAC Origen y la Dirección IP solicitada NO coindicen\ncon el registro en la BDD.....(FAILED)\n");
            		      printf("================================================="RESET);
            		      printf (GRN"\n    ARP Gratuitous \"saliente\" (Respuesta)");
                      estructuraTrama (trama_envio, MAC_THAi, MACd, ethtype, HW_PR, HS_PS, OPCODE_REPLY,
                        MACd,IP_SPAi, MAC_THAi, IP_TPAi);
                      imprimeTrama(trama_envio, tam);
                      printf(MAG"Dirección MAC objetivo"RESET" >> "BLU);
                      fprintf(stdout,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n"
                        ,MAC_THAi[0],MAC_THAi[1],MAC_THAi[2],MAC_THAi[3],MAC_THAi[4],MAC_THAi[5]);
                      printf(RED"================================================="RESET);
                      enviarTrama (trama_envio, TRAMA_LEN, ds, indice);
                      printf(YEL"\nARP Gratuitous a broadcast \"saliente\" (Petición)");
                      estructuraTrama(trama_envio, MAC_DD, MACd, ethtype, HW_PR, HS_PS, OPCODE_REQ,
                        MACd, IP_SPAi, THA_NULL, IP_TPAi);
                      imprimeTrama(trama_envio, tam);
                      printf(MAG"Dirección MAC objetivo"RESET" >> "BLU);
                      fprintf(stdout,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n"
                        ,MAC_DD[0],MAC_DD[1],MAC_DD[2],MAC_DD[3],MAC_DD[4],MAC_DD[5]);
                      printf(RESET);
                      enviarTrama (trama_envio, TRAMA_LEN, ds, indice);
            		      break;
            		    }
        		   }
	         }
	   }
  }
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
      exit(EXIT_FAILURE);

}

void
estructuraTrama (unsigned char *trama_envio, unsigned char *MACdestino, 
     unsigned char *MACOrigen,
     unsigned char *ethtype,
     unsigned char *HW_PR, unsigned char * HS_PS, unsigned char *OPCODE,
     unsigned char *SHA, unsigned char *SPA,
     unsigned char *THA, unsigned char *TPA)
{
  memset (trama_envio, 0x00, TRAMA_LEN);
  memcpy (trama_envio + 0, MACdestino, 6);  //Direccion Destino (6 bytes)
  memcpy (trama_envio + 6, MACOrigen, 6); //Direccion Origen (6 bytes)
  memcpy (trama_envio + 12, ethtype, 2);  //Ethertype (2 bytes)
  //Encabezado ARP
  memcpy (trama_envio + 14, HW_PR, 4); //HW  
  memcpy (trama_envio + 18, HS_PS, 2); //PR
  memcpy (trama_envio + 20, OPCODE, 2); //OPCODE
  memcpy (trama_envio + 22, SHA, 6);  //SHA
  memcpy (trama_envio + 28, SPA, 4); //SPA
  memcpy (trama_envio + 32, THA, 6); //THA
  memcpy (trama_envio + 38, TPA, 4); //TPA
}

void
finish_with_error (MYSQL * con)
{
  fprintf (stderr, "Error al conectar con MySQL:%s\n", mysql_error (con));
  mysql_close (con);
  exit (EXIT_FAILURE);
}

void
conectaDB (MYSQL * con)
{ 
  char user[15];
  char pass[15];
  printf ("MySQL user >> ");
  scanf ("%s", user);
  printf ("MySQL password >> ");
  flush_in ();
  getPassword (pass);
   if (con == NULL)
     finish_with_error (con);
    if (mysql_real_connect (con, "localhost", user, pass,
			    "analizadorARP", 0, NULL, 0) == NULL)
      {
        	finish_with_error (con);
      }

    if (mysql_query (con, "USE analizadorARP"))
      {
	       finish_with_error (con);
      }
  printf ("\n");
}

void getPassword(char * password)
{
    static struct termios oldt, newt;
    int i = 0;
    int c;

    /*saving the old settings of STDIN_FILENO and copy settings for resetting*/
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;

    /*setting the approriate bit in the termios struct*/
    newt.c_lflag &= ~(ECHO);          

    /*setting the new bits*/
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*reading the password from the console*/
    while ((c = getchar())!= '\n' && c != EOF && i < 15){
        password[i++] = c;
    }
    password[i] = '\0';

    /*resetting our old STDIN_FILENO*/ 
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

}

void flush_in(void) 
{ 
int ch; 
while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){} 
}

void
convertirMACa6bytes (unsigned char *MAC_bytes, MYSQL_ROW * MAC_string)
{
  int values[6];
  int i;

  if (6 == sscanf (**MAC_string, "%x:%x:%x:%x:%x:%x",
		   &values[0], &values[1], &values[2],
		   &values[3], &values[4], &values[5]))
    {
      /* convert to uint8_t */
      for (i = 0; i < 6; ++i)
	MAC_bytes[i] = (unsigned char) values[i];
    }

  else
    {
      printf ("No pude convertir\n");
      exit(EXIT_FAILURE);
    }
}

int main (void){
	int ds, indice = 0, OC4;
unsigned char ownMAC[6], ownIP[4], ownBroadcast[4], ownNetMask[4];
struct ifreq Interfaz;
struct in_addr IPDestino;
unsigned char trama_recibir[1514], trama_enviar[1514];

MYSQL *con = mysql_init (NULL);
conectaDB(con);

ds=abrirSocketRaw();
indice=obtenerDatos(ds, &Interfaz, ownMAC, ownIP, ownBroadcast, ownNetMask);

while(1){
  recibeTrama (trama_recibir, ds, indice, trama_enviar, con);
}

	return 0;
}