#include <pathping.h>
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
    printf("\nCreado ruta hacia %s (%s), %d saltos max, paquetes de (%d) bytes.\n\n" , hostname , ipstring_dest, MAXHOP,icmpdatabytes);
    IPstringToArray(ipstring_dest, IP_dest);
    usleep(200);
    if(hostIsInNetwork(ownIP, ownNetMask, IP_dest))
        ARP(ownMAC, ownIP, IP_dest, targetHwAddr, &ifindex);
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
    node_t * actual = cabeza;
    int nodostotales = cuentaNodos(cabeza);
    //printf("\n");
    //print_list(cabeza);
    printf("\nComputando datos; envío de %d paquetes\n", nodostotales*100*2);
    unsigned int seq=1;
    struct timeval tval_after, tval_result, tval_now; 
    int x = 0;
    unsigned int ttlsistema = getTTL();
        for(x=1; x<=100; x++){
            node_t * actual = cabeza;
            while(actual!=NULL){
                memset(&tval_now, 0, sizeof(tval_now));
                gettimeofday(&tval_now, NULL);
                if(PING(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, actual->IPlist, seq, pID, ds, ifindex, actual->TTLlist)){
                    memset(&tval_after, 0, sizeof(tval_after));
                    memset(&tval_result, 0, sizeof(tval_result));
                    gettimeofday(&tval_after, NULL);
                    timersub(&tval_after, &tval_now, &tval_result);
                    actual->tiempo_ttl_c = actual->tiempo_ttl_c + (long int)(tval_result.tv_usec/1000.0);
                    actual->contestados_ttl_c=actual->contestados_ttl_c+1;
                }
            actual = actual->siguiente;
            seq++;
            }
        }
    //printf("\n");
    //print_list(cabeza);
    seq=1;
    for(x=1; x<=100; x++){
        node_t * actual = cabeza;
        while(actual!=NULL){
            gettimeofday(&tval_now, NULL);
            if(PING(trama_icmp, (int)sizeof(trama_icmp), ICMP_PROT, ownIP, actual->IPlist, seq, pID, ds, ifindex, ttlsistema)){
                memset(&tval_after, 0, sizeof(tval_after));
                memset(&tval_result, 0, sizeof(tval_result));
                gettimeofday(&tval_after, NULL);
                timersub(&tval_after, &tval_now, &tval_result);
                actual->tiempo_ttl_system = actual->tiempo_ttl_system + (long int)(tval_result.tv_usec/1000.0);
                actual->contestados_ttl_system=actual->contestados_ttl_system+1;
            }
        actual = actual->siguiente;
        seq++;
        }
    }
    //printf("\n");
    //print_list(cabeza);
    printf("\n");
    printf("\n   \t   TTL Justa   \t\t\t        TTL %d\n", ttlsistema);
    printf("HOP\tPerdidos/Enviados\tTiempo\t   Perdidos/Enviados\tTiempo\t  Dirección\n\n");
    actual = cabeza;
    imprimeResultados(actual);
    actual=cabeza;
    chingateLaLista(&actual);
    close(ds);
    return 0;   
}