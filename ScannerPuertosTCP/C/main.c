#include <protocolos.h>

int main(int argc , char **argv)
{
    if(geteuid()!=0){
        printf("%s debe ser ejecutado con privilegios root\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(argc != 2)
        usage(argv);
    char *hostname = argv[1];
    char ipstring_dest[17], InterfazName[10];
    unsigned char ownMAC[6], ownIP[4], ownNetMask[4];
    unsigned char targetHwAddr[6], IP_dest[4];
    int ds, ifindex = 0;
    struct ifreq Interfaz;
    unsigned char trama_tcp[PACKETTOTLEN];
    unsigned char trama_rcv[100];
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
    IPstringToArray(ipstring_dest, IP_dest);
    printf("Escanenado puertos TCP 1 - 100\n");
    printf(YEL"\nPUERTO\tESTADO\n\n"RESET);
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
   // int puertodestino = 80 ;   
    for(int puertodestino = 1; puertodestino<=100; puertodestino++){
        sleep(1);
        setEthHeader (trama_tcp, targetHwAddr, ownMAC, ETHTYPE_IP, (int)sizeof(trama_tcp));
        setIPHeader(trama_tcp, TCP_PROT, ownIP, IP_dest, puertodestino);
        setTCPHeader(trama_tcp, htons(61000), htons(puertodestino), ownIP, IP_dest);
        enviarTrama (trama_tcp, (int)sizeof(trama_tcp), ds, ifindex);
        printf(BLU"  %d", puertodestino);
        printf(RESET"");
        usleep(300);
        memset(&trama_rcv, 0, sizeof(trama_rcv));
        rcvTCP(trama_rcv, sizeof(trama_rcv), ds, ifindex, htons(61000), htons(puertodestino));
        sleep(1);
    }
    printf("Bye\n");
    close(ds);
    return 0;   
}