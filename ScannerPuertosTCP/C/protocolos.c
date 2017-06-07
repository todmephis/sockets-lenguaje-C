#include <protocolos.h>
                        /*FUNCIONES*/
void usage (char ** arguments){
    printf("Usage: %s [ip/hostname]\n", arguments[0]);
    exit(EXIT_FAILURE);
}
 
int hostnameToIP(char * hostname , char* ip){
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ( (he = gethostbyname( hostname ) ) == NULL) 
        return -1;
    else
    { 
    	addr_list = (struct in_addr **) he->h_addr_list;
	    for(i = 0; addr_list[i] != NULL; i++) 
    	    {
    	        strcpy(ip , inet_ntoa(*addr_list[i]));
    	        return 1;
	        }
	} 
    return 1;
}

const char * getHostnamefromIP(char * ip){
struct sockaddr_in sa;    /* input */
    socklen_t len;         /* input */
    char hbuf[NI_MAXHOST];
    const char * string_to_return;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *) &sa, len, hbuf, sizeof(hbuf), 
        NULL, 0, NI_NAMEREQD)) {
        strcpy(ip, "\b");
        return ip;
    }
    else {
        //printf("host=%s\n", hbuf);
        string_to_return = (const char *)malloc(sizeof(char) * ((int)strlen(hbuf)+1));
        strcpy((char *)string_to_return, hbuf);
        return strdup(string_to_return);
    }
}

int IPstringToArray(char * ipstring, unsigned char * iparray){
	char * token;
	int i=0;
	token=strtok(ipstring, ".");
	while(token != NULL)
    {
		iparray[i]=atoi(token);
		token = strtok(NULL, ".");
		i++;
	}
    return 0;
}

int abrirSocketRaw(void){
    int packet_socket = 0;
    packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(packet_socket == -1)
    {
        perror("Error al abrir el socket");
        exit (EXIT_FAILURE);
    }
    return packet_socket; 
}

int obtenerDatos(int ds, struct ifreq * ptrInterfaz, unsigned char * ownMAC, unsigned char * ownIP, unsigned char * ownNetMask){
int index=0;
if(ioctl (ds, SIOCGIFINDEX, ptrInterfaz) == -1)
    {
    perror("Error al obtener el índice");
    exit (EXIT_FAILURE);
    }
else
    {
    index = ptrInterfaz->ifr_ifindex;
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
    }   
//Dirección IP
if (ioctl (ds, SIOCGIFADDR, ptrInterfaz) == -1)
    {
      perror ("Error obteniendo la dirección IP");
      exit (EXIT_FAILURE);
    }
else
    {
      memcpy (ownIP, ptrInterfaz->ifr_addr.sa_data + 2, 4);
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
    }

return index;
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

int hostIsInNetwork(unsigned char *miIP, unsigned char *mascara, unsigned char *host){
    unsigned char netID[4];
    unsigned char ANDing[4];
    int i;
    memcpy(netID, miIP, 4);
    memset(&netID[3], 0, sizeof(char));
    for (i = 0; i < 4; i++)
    {
        ANDing[i]=host[i]&mascara[i];
    }
    if(!memcmp(ANDing, netID, 4)){
        return 1;
    }
    else
        return 0;
}
int getGatewayAddr(unsigned char * gatewayIParray)
{
    int     received_bytes = 0, msg_len = 0, route_attribute_len = 0;
    int     sock = -1, msgseq = 0;
    struct  nlmsghdr *nlh, *nlmsg;
    struct  rtmsg *route_entry;
    struct  rtattr *route_attribute;
    char    gateway_address[INET_ADDRSTRLEN], interface[IF_NAMESIZE];
    char    msgbuf[BUFFER_SIZE], buffer[BUFFER_SIZE];
    char    *ptr = buffer;
    struct timeval tv;

    if ((sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    memset(msgbuf, 0, sizeof(msgbuf));
    memset(gateway_address, 0, sizeof(gateway_address));
    memset(interface, 0, sizeof(interface));
    memset(buffer, 0, sizeof(buffer));
    nlmsg = (struct nlmsghdr *)msgbuf;
    nlmsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    nlmsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .
    nlmsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
    nlmsg->nlmsg_seq = msgseq++; // Sequence of the message packet.
    nlmsg->nlmsg_pid = getpid(); // PID of process sending the request.
    tv.tv_sec = 1;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
    /* send msg */
    if (send(sock, nlmsg, nlmsg->nlmsg_len, 0) < 0) {
        perror("send failed");
        return EXIT_FAILURE;
    }
    do
    {
        received_bytes = recv(sock, ptr, sizeof(buffer) - msg_len, 0);
        if (received_bytes < 0) {
            perror("Error in recv");
            return EXIT_FAILURE;
        }

        nlh = (struct nlmsghdr *) ptr;
        if((NLMSG_OK(nlmsg, received_bytes) == 0) ||
           (nlmsg->nlmsg_type == NLMSG_ERROR))
        {
            perror("Error in received packet");
            return EXIT_FAILURE;
        }
        if (nlh->nlmsg_type == NLMSG_DONE)
            break;
        else {
            ptr += received_bytes;
            msg_len += received_bytes;
        }

        if ((nlmsg->nlmsg_flags & NLM_F_MULTI) == 0)
            break;
    }
    while ((nlmsg->nlmsg_seq != msgseq) || (nlmsg->nlmsg_pid != getpid()));

    for ( ; NLMSG_OK(nlh, received_bytes); nlh = NLMSG_NEXT(nlh, received_bytes))
    {
        route_entry = (struct rtmsg *) NLMSG_DATA(nlh);
        if (route_entry->rtm_table != RT_TABLE_MAIN)
            continue;

        route_attribute = (struct rtattr *) RTM_RTA(route_entry);
        route_attribute_len = RTM_PAYLOAD(nlh);
        for ( ; RTA_OK(route_attribute, route_attribute_len);
              route_attribute = RTA_NEXT(route_attribute, route_attribute_len))
        {
            switch(route_attribute->rta_type) {
            case RTA_OIF:
                if_indextoname(*(int *)RTA_DATA(route_attribute), interface);
                break;
            case RTA_GATEWAY:
                inet_ntop(AF_INET, RTA_DATA(route_attribute),
                          gateway_address, sizeof(gateway_address));
                break;
            default:
                break;
            }
        }

        if ((*gateway_address) && (*interface)) {
            //fprintf(stdout, "Gateway %s for interface %s\n", gateway_address, interface);
            IPstringToArray(gateway_address, gatewayIParray);
            break;
        }
    }

    close(sock);
    return 0;

}
void setEthHeader (unsigned char *trama, unsigned char *destHwAddr, unsigned char *sourceHwAddr, unsigned char *ethType, int trama_len){
    memset((void*)trama, 0, trama_len);
    memcpy(trama+0, destHwAddr, 6);
    memcpy(trama+6, sourceHwAddr, 6);
    memcpy(trama+12, ethType, 2);
}
void setARPHeader (unsigned char *trama, unsigned char * sourceIPAddr, unsigned char * targetIPAddr){
    memcpy(trama+14, "\x00\x01", 2);//Hardware Type
    memcpy(trama+16, "\x08\x00", 2);//Protocol Type
    memcpy(trama+18, "\x06", 1);//Hardware Address Length
    memcpy(trama+19, "\x04", 1);//Protocol Address Length
    memcpy(trama+20, "\x00\x01", 2);//Operation Code
    memcpy(trama+22, trama+6, 6);//Source Hardware Address
    memcpy(trama+28, sourceIPAddr, 4);//Source Protocol Address
    memcpy(trama+38, targetIPAddr, 4);//Target Protocol Address
}

void setIPHeader(unsigned char * trama, unsigned int protocol, unsigned char * sourceIPAddr, unsigned char *targetIPAddr, int count){
    struct timeval time; 
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 10000) + (time.tv_usec / 10000));
    unsigned char identifier[2];
    unsigned char checkSum[2];
    unsigned short ckSum;
    unsigned int TTL=getTTL();
    unsigned char * ptr, *ptr1;
    int id=(rand()%(89999+1)+10000) + (count+1); //La dvd no se que mierdas hice aquí, ya no me acuerdo :v
    ptr =(char *) &id;
    ptr1=ptr;
    for(&ptr; *ptr; ptr++){
        identifier[(int)(ptr-ptr1)] = *ptr;
    }
    memcpy(trama+14, "\x45", 1);//version y longitud de encabezado
    memset(trama+17, IPTOTLEN, 1);//Total Length
    memset(trama+18, identifier[1], 1);//Identificador
    memset(trama+19, identifier[0], 1);//Identificador
    memcpy(trama+20, "\x40", 1);//Flags 0x40 is equal to "do not fragment"
    memset(trama+22, TTL, 1);//Time To live
    memset(trama+23, protocol, 1);////Protocol0
    memset(trama+24, 0, 2);//SET CHECKSUM 0
    memcpy(trama+26, sourceIPAddr, 4);//Source IP Address
    memcpy(trama+30, targetIPAddr, 4);//Target IP Address
    //SET CHECKSUM
    unsigned char tempIPHead[20];
    memcpy(tempIPHead, trama+14, (int)sizeof(tempIPHead));
    ckSum=checksum(tempIPHead, sizeof(tempIPHead));
    ptr =(char *) &ckSum;
    ptr1=ptr;
    for(&ptr; *ptr; ptr++){
        checkSum[(int)(ptr-ptr1)] = *ptr;
    }
    memset(trama+24, checkSum[1], 1);
    memset(trama+25, checkSum[0], 1);
}
void setTCPHeader(unsigned char * trama, unsigned int sourceport, unsigned int destport, unsigned char * sourceIPAddr, unsigned char * targetIPAddr){
    int len = PACKETTOTLEN-34;
    len = htons(len);
    memcpy(trama+34, (char *) &sourceport, 2);//Source port
    memcpy(trama+36, (char *) &destport, 2);//Destination port
    memcpy(trama+38, "\xb7\x47\x4c\xcc", 4);//Sequence Number
    memset(trama+42, 0, 4);//ACK
    memcpy(trama+46, "\x70\x02", 2); //Longitud de encabezado y bandera SYN encendida
    memcpy(trama+48, "\x40\x00", 2); //Winsize
    memset(trama+50, 0, 2); //Set checksum field to zero
    memset(trama+52, 0, 2); //Urgent pointer disabled
    memcpy(trama+54, "\x02\x04\x05\xb4\x01\x01\x04\x02", 8);
    //CHECKSUM
    //PSEUDOHEADER
    unsigned char pseudoheader[12];
    memset(&pseudoheader, 0, sizeof(pseudoheader));
    memcpy(pseudoheader+0, sourceIPAddr, 4);
    memcpy(pseudoheader+4, targetIPAddr, 4);
    memset(pseudoheader+9, 6, 1);
    memcpy(pseudoheader+10, (char *)&len, 2);
    //Header
    unsigned char tempTCPHeader[40];
    memset(&tempTCPHeader, 0, sizeof(tempTCPHeader));
    memcpy(tempTCPHeader+0, pseudoheader, 12);
    memcpy(tempTCPHeader+12, trama+34, 28);
    unsigned short ckSum = checksum(tempTCPHeader, (int)sizeof(tempTCPHeader));
    ckSum = htons(ckSum);
    memcpy(trama+50, (char *)&ckSum, 2); //Set checksum;


}
void setUDPHeader(unsigned char * trama, unsigned int sourceport, unsigned int destport, unsigned char * sourceIPAddr, unsigned char * targetIPAddr){
    int len = PACKETTOTLEN-34;
    len = htons(len);
    memcpy(trama+34, (char *) &sourceport, 2);//Source port
    memcpy(trama+36, (char *) &destport, 2);//Destination port
    memcpy(trama+38, (char *) &len, 2);//Lenght
    memset(trama+40, 0, 2);//Set checksum field on zero.
    //CHECKSUM
    //PSEUDOHEADER
    unsigned char pseudoheader[12];
    memset(&pseudoheader, 0, sizeof(pseudoheader));
    memcpy(pseudoheader+0, sourceIPAddr, 4);
    memcpy(pseudoheader+4, targetIPAddr, 4);
    memset(pseudoheader+8, 0, 1);
    memset(pseudoheader+9, 17, 1);
    memcpy(pseudoheader+10, (char *)&len, 2);
    unsigned char tempUDPHeader[40];
    memset(&tempUDPHeader, 0, sizeof(tempUDPHeader));
    memcpy(tempUDPHeader+0, pseudoheader, 12);
    memcpy(tempUDPHeader+12, trama+34, 28);
    unsigned short ckSum = checksum(tempUDPHeader, (int)sizeof(tempUDPHeader));
    ckSum=htons(ckSum);
    memcpy(trama+40, (char *)&ckSum, 2);//Mejorando el flow del código :v ya no más apuntadores a enteros
}
void setICMPHeader(unsigned char * trama, unsigned short seqNumber, int ID){
    unsigned char identifier[2];
    unsigned char checkSum[2];
    unsigned char data[48]={"IVAN IVAN IVAN IVAN IVAN IVAN :V IVAN :V IVAN :v"};//payload
    unsigned short ckSum;
    memcpy(trama+34, "\x08", 1);//Type ECHO req
    unsigned char * ptr, *ptr1;
    ptr =(char *) &ID;
    ptr1=ptr;
    for(&ptr; *ptr; ptr++){
        identifier[(int)(ptr-ptr1)] = *ptr;
    }
    memset(trama+36, 0, 2);//SET CHECKSUM 0
    memset(trama+38, identifier[1], 1);//Identificador
    memset(trama+39, identifier[0], 1);//Identificador
    memset(trama+41, seqNumber, 1);//Sequence Number
    //memset(trama+42, TMESTAMP FIELD, 8); //TO DO: SET TIMESTAMP :'V
    memcpy(trama+50, data, sizeof(data));
    //SET CHECKSUM
    unsigned char tempICMPHead[64];
    memcpy(tempICMPHead, trama+34, (int)sizeof(tempICMPHead));
    ckSum=checksum(tempICMPHead, (int)sizeof(tempICMPHead));
    ptr =(char *) &ckSum;
    ptr1=ptr;
    for(&ptr; *ptr; ptr++){
        checkSum[(int)(ptr-ptr1)] = *ptr;
    }
    memset(trama+36, checkSum[1], 1);//Colocamos el checksum a la trama.
    memset(trama+37, checkSum[0], 1);
}

unsigned short checksum (unsigned char * buff, int bufflen){
    unsigned short cksum=0;
    unsigned short acarreo=0;
    int i, suma = 0, resultado=0,temp=0;
    for(i=0; i<bufflen; i=i+2){
        temp=(buff[i]<<8)+buff[i+1];
        suma = suma+temp;
        temp=0;
    }
    acarreo=suma>>16;
    resultado = (suma&0x0000FFFF)+acarreo;
    acarreo=resultado>>16;
    resultado = (resultado&0x0000FFFF)+acarreo;
    cksum =0xffff-resultado;
    return cksum;
}

unsigned int getTTL(void){
    int i=0;
    char x;
    unsigned char stringttl[4];
    FILE * filettl = NULL;

    filettl = fopen(TTLPATH, "r");

    if(filettl == NULL){
        perror("No se pudo abrir el archivo:");
        exit (EXIT_FAILURE);
    }
    fread(&x, sizeof(char), 1, filettl);
    while(!feof(filettl)){
        stringttl[i]=x;
        fread(&x, sizeof(char), 1, filettl);
        i++;
    }
    fclose(filettl);
    return atoi(stringttl);
}

void cpyHwAddrFromTrama(unsigned char *trama, unsigned char * destHwAddr){
    memcpy(destHwAddr, trama+22, 6);
}


void ARP(unsigned char * sourceHwAddr, unsigned char * sourceIPAddr, unsigned char * targetIPAddr, unsigned char * targetHwAddr, int * ifindex){
    int ds=abrirSocketRaw();
    unsigned char trama_env[42];
    unsigned char trama_rcv[60];
    int trama_env_len=(int)sizeof(trama_env), trama_rcv_len=(int)sizeof(trama_rcv);
    sendARP(trama_env, sourceHwAddr, sourceIPAddr, targetIPAddr, trama_env_len, ifindex, ds);
    rcvARP(trama_rcv, sourceHwAddr, targetIPAddr, trama_rcv_len, ifindex, ds, targetHwAddr);
}

int sendARP(unsigned char * trama_env, unsigned char * sourceHwAddr, unsigned char * sourceIPAddr, unsigned char * targetIPAddr, int trama_len, int * ifindex, int ds){
    setEthHeader(trama_env, BROADHWADDR, sourceHwAddr, ETHTYPE_ARP, trama_len);
    setARPHeader(trama_env, sourceIPAddr, targetIPAddr);
    enviarTrama(trama_env, trama_len, ds, *ifindex);
}

void rcvARP(unsigned char * trama_rcv, unsigned char * sourceHwAddr, unsigned char * targetIPAddr, int trama_len, int * ifindex, int ds, unsigned char * targetHwAddr){
    struct timeval start, end;
    int tam_rcv_from, bandera=0;
    long mtime =0, seconds, useconds;
    gettimeofday(&start, NULL);
    while(mtime<400){
        tam_rcv_from=recibeTrama(ds, trama_rcv, trama_len);
        if(filterARPreply(trama_rcv, trama_len, sourceHwAddr, targetIPAddr, tam_rcv_from)==1){
            cpyHwAddrFromTrama(trama_rcv, targetHwAddr);
            bandera = 1;
            close(ds);
        }
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds)*1000 + useconds/1000.0) + 0.5;
        if(bandera==1)
            break;
    }
    if (bandera == 0){
      perror (RESET"Sin respuestaaa");
      exit(EXIT_FAILURE);
    }
}
/*int ICMP(unsigned char *trama_icmp, int trama_len, int protocolNumber,unsigned char *destHwAddr, unsigned char *sourceHwAddr, unsigned char * sourceIPAddr, unsigned char * targetIPAddr, int sqNumber, int pID, int ds, int index){
    struct timeval tval_now;
    unsigned char trama_rcv[100];
    setIPHeader(trama_icmp, protocolNumber, sourceIPAddr, targetIPAddr, sqNumber);
    setICMPHeader(trama_icmp, sqNumber, pID);
    gettimeofday(&tval_now, NULL);
    enviarTrama (trama_icmp, trama_len, ds, index);
    if(rcvICMP(trama_rcv, trama_len, destHwAddr, sourceHwAddr, targetIPAddr, sourceIPAddr,  pID,  sqNumber,  ds, index, &tval_now))
        return 1;
    else
        return 0;

}*/

int rcvICMP(unsigned char *trama_rcv, int trama_len, int ds, int index, unsigned int sourcePort, unsigned int destPort){
    struct timeval start, end;
    int bandera=0;
    long mtime =0, seconds, useconds;
    gettimeofday(&start, NULL);
    while(mtime<5000){
        recibeTrama(ds, trama_rcv, trama_len);
        if(filterICMPreply(trama_rcv, sourcePort, destPort)==0){
            printf(RED"\tCERRADO");
            printf(RESET"\n");
            //imprimeTrama(trama_rcv, trama_len);
            bandera = 1;
        }
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds)*1000 + useconds/1000.0) + 0.5;
        if(bandera==1){
            return 1;
            break;
        }
    }
    if (bandera == 0){
      printf(GRN"\tABIERTO");
      printf(RESET"\n");
      return 0;
    }
}

int filterICMPreply(unsigned char *trama, unsigned int sourcePort, unsigned int destPort){
    if(!memcmp (trama + 12, ETHTYPE_IP, 2) && !memcmp (trama + 34, "\x03\x03", 2) && !memcmp (trama + 62, (char*)&sourcePort, 2)
        && !memcmp(trama+64, (char *)&destPort, 2)){
        return 0;
    }
    else
    return 1;
}

int rcvTCP(unsigned char *trama_rcv, int trama_len, int ds, int index, unsigned int sourcePort, unsigned int destPort){
    struct timeval start, end;
    int bandera=0;
    long mtime =0, seconds, useconds;
    gettimeofday(&start, NULL);
    while(mtime<500){
        recibeTrama(ds, trama_rcv, trama_len);
        if(filterTCPreply(trama_rcv, sourcePort, destPort)==0){
            printf(RED"\tCERRADO");
            printf(RESET"\n");
            //imprimeTrama(trama_rcv, trama_len);
            bandera = 1;
        }
        else if (filterTCPreply(trama_rcv, sourcePort, destPort)==1){
            printf(GRN"\tABIERTO");
            printf(RESET"\n");
            //imprimeTrama(trama_rcv, trama_len);
            bandera = 1;
        }
        else if(filterICMPreply(trama_rcv, sourcePort, destPort)==0){
            printf(RED"\tCERRADO (ICMP Destination Unreachable, Destination port unreachable)");
            printf(RESET"\n");
            //imprimeTrama(trama_rcv, trama_len);
            bandera = 1;
        }
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds)*1000 + useconds/1000.0) + 0.5;
        if(bandera==1){
            return 1;
            break;
        }
    }
    if (bandera == 0){
      //printf(RED"\tSIN RESPUESTA");
      printf(RESET"\n");
      return 0;
    }
}

int filterTCPreply(unsigned char *trama, unsigned int sourcePort, unsigned int destPort){
    unsigned char puertos[4];
    memcpy(puertos+0, (char *)&destPort, 2);
    memcpy(puertos+2, (char *)&sourcePort, 2);
    if(!memcmp(trama+12, ETHTYPE_IP, 2) && !memcmp(trama+23, "\x06", 1) && !memcmp(trama+34, puertos, 4) && !memcmp(trama+47, "\x014", 1)){
        return 0;
    }
    else if(!memcmp(trama+12, ETHTYPE_IP, 2) && !memcmp(trama+23, "\x06", 1) && !memcmp(trama+34, puertos, 4) && !memcmp(trama+47, "\x012", 1)){
        return 1;
    }
    else
        return -1;
}


void enviarTrama (unsigned char *trama_enviar, int tramalen, int ds, int index)
{
  int tam;
  struct sockaddr_ll interfaz;
  memset (&interfaz, 0x00, sizeof (interfaz));
  interfaz.sll_family = AF_PACKET;
  interfaz.sll_protocol = htons (ETH_P_ALL);
  interfaz.sll_ifindex = index;
  tam = sendto (ds, trama_enviar, tramalen, 0, (struct sockaddr *) &interfaz,
      sizeof (interfaz));
  if (tam == -1)
    {
      perror ("Sin respuesta");
      //exit(EXIT_FAILURE);
    }
}

int recibeTrama(int ds, unsigned char * trama, int trama_len)
{
    int tam=0;
    tam=recvfrom(ds, trama, trama_len, MSG_DONTWAIT, NULL, 0);
    return tam;
}
int filterARPreply(unsigned char *trama, int trama_len, unsigned char * destHwAddr, unsigned char * sourceIPAddr, int trama_rcv_from_len){
    if(!memcmp (trama + 0, destHwAddr, 6)
       && !memcmp (trama + 12, ETHTYPE_ARP, 2)
       && !memcmp (trama + 20, "\x00\x02", 2)
       && !memcmp (trama + 28, sourceIPAddr, 4)
       && !memcmp (trama + 32, destHwAddr, 6)){
        return 1;
    }
    return -1;
}

void imprimeTrama (unsigned char *trama, int tramalen)
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