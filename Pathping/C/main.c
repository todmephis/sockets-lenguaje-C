#include <traceroute.h>
int main(int argc , char **argv)
{
    if(geteuid()!=0){
        printf("%s debe ser ejecutado con privilegios root\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(argc != 2)
        usage(argv);
    node_t * cabeza = NULL;
    char *hostname = argv[1];
    char ipstring_dest[17], InterfazName[10];
    unsigned char ownMAC[6], ownIP[4], ownNetMask[4];
    unsigned char targetHwAddr[6], IP_dest[4];
    int ds, ifindex = 0, pID;
    struct ifreq Interfaz;
    unsigned char trama_icmp[74];
    int icmpdatabytes = sizeof(trama_icmp)-14;
    cabeza = malloc(sizeof(node_t));
    if(cabeza == NULL){
        perror("Error reservando memoria");
        exit(EXIT_FAILURE);
        }
    if(hostnameToIP(hostname, ipstring_dest)== -1)
    {
        printf("Error: Dirección Inválida...\n");
        usage(argv);
    }
    ds=abrirSocketRaw();
    if(getDefaultInterfaz(InterfazName) == -1)
        exit(EXIT_FAILURE);
    strcpy(Interfaz.ifr_name, InterfazName);
    ifindex = obtenerDatos(ds, &Interfaz, ownMAC, ownIP, ownNetMask);
    printf("Traceroute a %s (%s), %d saltos max, paquetes de (%d) bytes.\n" , hostname , ipstring_dest, MAXHOP,icmpdatabytes);
    IPstringToArray(ipstring_dest, IP_dest);
    usleep(200);
    if(hostIsInNetwork(ownIP, ownNetMask, IP_dest))
        {
            ARP(ownMAC, ownIP, IP_dest, targetHwAddr, &ifindex);

        }
    else
        {
            unsigned char gatewayIP[4];
            getGatewayAddr(gatewayIP);
            ARP(ownMAC, ownIP, gatewayIP, targetHwAddr, &ifindex);
        }
    setEthHeader (trama_icmp, targetHwAddr, ownMAC, ETHTYPE_ICMP, (int)sizeof(trama_icmp));
    pID=getpid()*2;
    int sec = 0 ;
    unsigned int ttl=1;
    int flag = 0;
    for(sec=1; ttl<=MAXHOP; sec++){
        flag = ICMP(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, IP_dest, sec, pID, ds, ifindex, ttl, 1, cabeza);
        if(flag == 1){
            printf("\n");
            break;
        }
        ttl++;
        printf("\n");
    }
    pop(&cabeza);
    //print_list(cabeza);
    node_t * actual = cabeza;
    int succesPacket1, succesPacket2, x = 0;
    unsigned int ttlsistema = getTTL();
    printf("\n   \t    TTL Justa   \t       TTL %d\n", ttlsistema);
    printf("HOP \tPerdidos/Enviados \t  Perdidos/Enviados \t Dirección\n");
    while(actual != NULL){
        succesPacket1 = 0;
        succesPacket2 = 0;
        for(x = 1; x<=100; x++){
            if(PING(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, actual->IPlist, x, pID, ds, ifindex, actual->TTLlist)){
                succesPacket1++;
            }
        }
       for(x = 1; x<=100; x++){
            if(PING(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, actual->IPlist, x, pID, ds, ifindex, ttlsistema)){
                succesPacket2++;
            }
        }
    printf("%d \t    %d/%d\t\t\t%d/%d\t\t %d.%d.%.d.%.d\n",actual->TTLlist, 100-succesPacket1, x-1, 100-succesPacket2, x-1, actual->IPlist[0], actual->IPlist[1], actual->IPlist[2], actual->IPlist[3]);
    actual = actual->siguiente;
    }

    close(ds);
    return 0;   
}