#include <analizadorllc.h>
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
//printf("Interfaz de red >> ");
//scanf("%s", ptrInterfaz->ifr_name);
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
     //printf ("Dirección de broadcast >> ");
     //fprintf(stdout, "%d.%d.%d.%d\n", ownBroadcast[0], ownBroadcast[1], ownBroadcast[2], ownBroadcast[3]);
    }
if (ioctl (ds, SIOCGIFNETMASK, ptrInterfaz) == -1)
    {
      perror ("Error al obtener la mascara");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownNetMask, ptrInterfaz->ifr_netmask.sa_data + 2, 4);
      //printf ("Máscara de subred >> ");
      //fprintf(stdout, "%d.%d.%d.%d", ownNetMask[0], ownNetMask[1], ownNetMask[2], ownNetMask[3]);
    }
  printf ("\n");  

return index;
}

void
imprimeTrama (unsigned char *trama, int tramalen)
{
  int i;
  printf("\n\n===========================================================================================================\n");
  printf("\nTrama IEEE 802.3\n");
  for (i = 0; i < tramalen; i++)
    {
        if (i % 16 == 0)
        printf ("\n");
      printf ("%.2x ", *(trama + i));

    }
  printf ("\n\n");
}	

void recibeTrama(int ds, unsigned char * trama){
int tam=0;
int lenght=0, type=-1;
while(1){
	tam=recvfrom(ds, trama, 1514, 0, NULL, 0);
 	lenght = trama[13] | trama[12] << 8;
	if(tam == -1)
		perror("Error al recibir la trama\n");
	if(lenght <= 1500)//FILTRO >> Recibo tramas que en el campo de longitud sea menor o igual a 1500.
		{
      analizaLLC(trama);
		}	
}
}
void leerArchivoLLC(FILE * archivo){
  unsigned char buff[500];
  unsigned char *trama;
  char hexbyte[3];
  int linea=1;
  int val=0, i=0;
  while(fgets(buff, sizeof(buff), archivo)){
    //printf("[%d]%s\n", linea, buff);
    i = 0;
    memset(&trama, 0, sizeof(trama));
    //printf("len %d\n", (int)(strlen(buff)-2)/2);
    trama=malloc((int)(strlen(buff)-2)/2);
    for(int a = 0; a < strlen(buff); a+=2){
      hexbyte[0]=buff[a];
      hexbyte[1]=buff[a+1];
      sscanf(hexbyte, "%x", &val);
      trama[i]=(unsigned char)val;
      i++;
    }
    imprimeTrama(trama, (int)(strlen(buff)-2)/2);
    analizaLLC(trama);
    linea++;
  }
  fclose(archivo);
}

void analizaLLC(unsigned char * tramaRecv){
  int i;
  //ENCABEZADO MAC
  printf("MAC Destino: ");
    printf("%.2x",tramaRecv[0]);
    for(i=1;i<6;i++){
      printf(":%.2x",tramaRecv[i]);
    }
    printf("\n");
  printf("MAC Origen: ");
    printf("%.2x",tramaRecv[6]);
    for(i=7;i<12;i++){
      printf(":%.2x",tramaRecv[i]);
    }
    printf("\n");
  printf("Longuitud: %d\n",tramaRecv[12]*256+tramaRecv[13]);
  //DSAP
  printf("\n::::::::::::::::::::::::::::::::::DSAP::::::::::::::::::::::::::::::::::\n\n");
  if((tramaRecv[14]&0x01)==1){printf("Trama de grupo\n");}
  else{printf("\t\t\t  Trama individual\n");}
  printf("\t\t\t  Protocolo: ");
    switch((tramaRecv[14]&0xFE)){
      case 0x04:printf("IBM SNA");break;
      case 0x06:printf("IP");break;
      case 0x80:printf("3COM");break;
      case 0xAA:printf("SNAP");break;
      case 0xBC:printf("BANYAN");break;
      case 0xE0:printf("NOVELL");break;
      case 0xFA:printf("LAN MANAGER FE-CLNS");break;
      case 0x42:printf("SPANNING TREE BPDU");break;
      case 0xF0:printf("NETBIOS");break;
    }
  printf("\n\n::::::::::::::::::::::::::::::::::SSAP::::::::::::::::::::::::::::::::::\n\n");
  if((tramaRecv[15]&0x01)==1){printf("\t\t\t  Trama de respuesta\n");}
  else{printf("\t\t\t  Trama de comando\n");}
  printf("\t\t\t  Protocolo: ");
    switch((tramaRecv[15]&0xFE)){
      case 0x04:printf("IBM SNA");break;
      case 0x06:printf("IP");break;
      case 0x80:printf("3COM");break;
      case 0xAA:printf("SNAP");break;
      case 0xBC:printf("BANYAN");break;
      case 0xE0:printf("NOVELL");break;
      case 0xFA:printf("LAN MANAGER FE-CLNS");break;
      case 0x42:printf("SPANNING TREE BPDU");break;
      case 0xF0:printf("NETBIOS");break;
    }
  printf("\n\n::::::::::::::::::::::::::::::::CONTROL:::::::::::::::::::::::::::::::::\n\n");
  int crtl = tramaRecv[16]&0x03;
  switch (crtl) {
    case 0:printf("Trama de informacion\n");break;
    case 2:printf("Trama de informacion\n");break;
    case 1:printf("Trama de supervision\n");break;
    case 3:printf("Trama no numerada\n");break;
  }
  if(crtl==0||crtl==2){
      printf("Extendida\nNúmero de secuencia de envio: %d\n",(tramaRecv[16]&0xFE)>>1);
      printf("Número de secuencia que se espera recibir: %d\n",(tramaRecv[17]&0xFE)>>1);
      printf("POLL/FINAL: %d ",tramaRecv[17]&0x01);
      //Vemos el estado del POLL/FINAL
      if(((tramaRecv[16]&0x10)>>4)==1){
        if((tramaRecv[15]&0x01)==1){printf("FINAL ");}
        else{printf("PULL ");}
        printf("Requiere respuesta inmediata\n");
      }
      else{printf("P/F Trama de informacion\n");}
  }else if(crtl==1){
      printf("Extendida\nNúmero de secuencia que se espera recibir: %d\n",(tramaRecv[17]&0xFE)>>1);
      //Determinamos que tipo de Trama de Supervision es
      switch((tramaRecv[16]&0x0C)){
        case 0:printf("Receiver ready (RR)");break;
        case 4:printf("Receiver not ready (RNR)");break;
        case 8:printf("Retransmicion (REJ)");break;
        case 12:printf("Retransmicion selectiva (SREJ)");break;
      }
      printf("POLL/FINAL: %d ",tramaRecv[17]&0x01);
      //Vemos el estado del POLL/FINAL
      if(((tramaRecv[16]&0x10)>>4)==1){
        if((tramaRecv[15]&0x01)==1){printf("FINAL ");}
        else{printf("PULL ");}
        printf("Requiere respuesta inmediata\n");
      }
      else{printf("P/F Trama de informacion\n");}
  }
  if(crtl==3){
    //No Numerada
    printf("Normal/Extendida\n");

    printf("POLL/FINAL: %d ",(tramaRecv[16]&0x10)>>4);
    //Vemos el estado del POLL/FINAL
    if(((tramaRecv[16]&0x10)>>4)==1){
      if((tramaRecv[15]&0x01)==1){//Trama de Respuesta es FINAL
        printf("FINAL Requiere respuesta inmediata\n");
        //Imprimimos acorde a lo que se plantea
        printf("\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((tramaRecv[16]&0xEC)){
          case 0x60:
            printf("|Unnumbered Acknowledgment UA|\t");
            printf("|Reconoce la conexion|\t");
            printf("|De uno de los comandos de modo de configuracion|\t");
          break;
          case 0x0C:
            printf("|Disconnect Mode DM|\t");
            printf("|Responder en modo de desconexión|\t");
            printf("|Modo de configuración requerido|\t");
          break;
          case 0x40:
            printf("|Request Disconnect RD|\t");
            printf("|Solicitud por el comando DISC|\t");
            printf("\t");
          break;
          case 0x04:
            printf("|Request Inialization Mode RIM|\t");
            printf("|Inicialización necesaria| \t");
            printf("|Solicitud de comando SIM|\t");
          break;
          case 0x84:
            printf("|Frame Reject FRMR|\t");
            printf("|Informe de Recepción|\t");
            printf("|De trama inaceptable|\t");
          break;
          default:
            printf("MMM MM INCORRECTO");
          break;
        }
      }
      else{//Trama de Comando es PULL
        printf("PULL Requiere respuesta inmediata\n");
        //Imprimimos acorde a lo que se plantea
        printf("\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((tramaRecv[16]&0xEC)){
          case 0x80:
            printf("|Set normal response SNRM|\t");
            printf("|Modo de Configuracion|\t");
            printf("|Usa 3 bits para números de secuencia|\t");
          break;
          case 0xCC:
            printf("|Set normal response extended mode SNRME|\t");
            printf("|Modo de Configuracion Extendido|\t");
            printf("|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x0C:
            printf("|Set asynchronous response SARM|\t");
            printf("|Modo de Configuracion Asincrono|\t");
            printf("|Usa 3 bits para números de secuencia|\t");
          break;
          case 0x4C:
            printf("|Set asynchronous response extended mode SARME|\t");
            printf("|Modo de Configuracion Asincrono Extendido|\t");
            printf("|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x2C:
            printf("|Set asynchronous balanced mode SABM|\t");
            printf("|Modo de Configuracion Asincrono Balanceadp|\t");
            printf("|Usa 3 bits para números de secuencia|\t");
          break;
          case 0x6C:
            printf("|Set asynchronous balanced extended mode SABME|\t");
            printf("|Modo de Configuracion Asincrono Balanceado Extendido|\t");
            printf("|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x04:
            printf("|Set inicialitation mode SIM|\t");
            printf("|Iniciar funcion de control de enlace|\t");
            printf("|Dirigido a la estacion|\t");
          break;
          case 0x40:
            printf("|Disconnect DISC|\t");
            printf("|Terminar la conexion de enlace lógico|\t");
            printf("|Para futuras tramas de I y S regresa DM|\t");
          break;
          case 0x20:
            printf("|Unnumbered Poll UP|\t");
            printf("|Se utiliza para solicitar|\t");
            printf("|Información de control|\t");
          break;
          case 0x8C:
            printf("|Reset RSET|\t");
            printf("|Utilizado para la recuperación|\t");
            printf("|Reestablece N(R), pero no N(S)|\t");
          break;
          default:
            printf("MMM MM INCORRECTO");
          break;
        }

      }
    }
    else{
      printf("P/F Trama de informacion\n");
      //Imprimimos acorde a lo que se plantea
      printf("\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
      switch((tramaRecv[16]&0xEC)){
        case 0x00:
          printf("|Unnumbered Information UI|\t");
          printf("|Datos no reconocidos|\t");
          printf("|Tiene una carga util|\t");
        break;
        case 0xAC:
          printf("|Exchange Identification XID|\t");
          printf("|Se utiliza para solicitar|\t");
          printf("|Información de control|\t");
        break;
        case 0xE0:
          printf("|Test TEST|\t");
          printf("|Intercambio de información idéntica|\t");
          printf("|Campos para la realización de pruebas|\t");
        break;
        default:
          printf("MMM MM INCORRECTO");
        break;
      }
    }
    printf("\n");
  }
}

void usage (char ** arguments){
    printf("Usage: %s [opcional nombre archivo]\n", arguments[0]);
    printf("Si no especifica nombre de archivo con tramas entonces recibirá tramas a través de la interfaz de red por defecto\n");
    exit(EXIT_FAILURE);
}

int getDefaultInterfaz(char * if_name){
    int i=0;
    struct ifaddrs *ifap, *ifa;
    //struct sockaddr_in *sa;
    //char *addr;
    if(getifaddrs (&ifap) != 0){
        perror("Error:");
        freeifaddrs(ifap);
        return -1;
    }
    else
    {
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family==AF_INET)
        {
            if(i==1)
            {
                strcpy(if_name, ifa->ifa_name);
            }
            i++;
        }
    }
    freeifaddrs(ifap);
    return 1;
    }
}