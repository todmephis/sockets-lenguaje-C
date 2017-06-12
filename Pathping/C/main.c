#include <traceroute.h>
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
    int ds, ifindex = 0, pID;
    struct ifreq Interfaz;
    unsigned char trama_icmp[74];
    int icmpdatabytes = sizeof(trama_icmp)-14;
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
    int flag = 0, printsetup=1;
    for(sec=1; ttl<=MAXHOP; sec++){
        flag = ICMP(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, IP_dest, sec, pID, ds, ifindex, ttl, printsetup);
        if(flag == 1){
            ICMP(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, IP_dest, sec+1, pID, ds, ifindex, ttl, printsetup+1);
            ICMP(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, IP_dest, sec+2, pID, ds, ifindex, ttl, printsetup+2);
            printf("\n");
            break;
        }
        if((sec)%3==0){
            ttl++;
            printf("\n");
        }
        printsetup++;
        if(printsetup==4)
            printsetup=1;
    }

    close(ds);
    return 0;   
}