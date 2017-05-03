#include <ping.h>

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
    unsigned char trama_icmp[98];
    int icmpdatabytes = sizeof(trama_icmp)-14;
    int succesPacket=0;
    float porcentaje=0.0;
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
    printf("PING %s (%s) 56(%d) bytes de datos.\n" , hostname , ipstring_dest, icmpdatabytes);
    IPstringToArray(ipstring_dest, IP_dest);
    sleep(1);
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
    signal(SIGINT, inthand);
    usleep(300);
    while(!stop){
        if(ICMP(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, targetHwAddr, ownMAC, ownIP, IP_dest, sec+1, pID, ds, ifindex)){
            succesPacket++;
         }
         sec++;
        sleep(1);
    }
    porcentaje=(((float)succesPacket/(float)(sec))*100.0);
    porcentaje = 100.0-porcentaje;
    printf("\n--- %s ping statics ---\n", hostname);
    printf("%d paquetes enviados, %d recibidos, %d%% paquetes perdidos\n", (sec), succesPacket, (int)porcentaje);
    close(ds);
    usleep(100);
    return 0;   
}