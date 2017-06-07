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
    unsigned char trama_udp[PACKETTOTLEN];
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
    printf("Escanenado puertos UDP 1 - 100\n");
    printf(YEL"PUERTO\tESTADO\n\n"RESET);
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
    setEthHeader (trama_udp, targetHwAddr, ownMAC, ETHTYPE_IP, (int)sizeof(trama_udp));
    int puertodestino;
    for(puertodestino = 1; puertodestino<=100; puertodestino++){
        //usleep(500);
        setIPHeader(trama_udp, UDP_PROT, ownIP, IP_dest, puertodestino);
        setUDPHeader(trama_udp, htons(61000), htons(puertodestino), ownIP, IP_dest);
        enviarTrama (trama_udp, (int)sizeof(trama_udp), ds, ifindex);
        printf(BLU"  %d", puertodestino);
        printf(RESET"");
        //usleep(300);
        memset(&trama_rcv, 0, sizeof(trama_rcv));
        rcvICMP(trama_rcv, sizeof(trama_rcv), ds, ifindex, htons(61000), htons(puertodestino));
        //usleep(500);
    }
    printf("Bye\n");
    close(ds);
    return 0;   
}