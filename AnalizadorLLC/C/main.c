#include <analizadorllc.h>
int main (int argc, char ** argv){
  if(argc == 1){
    if(geteuid()!=0){
      printf("%s debe ser ejecutado con privilegios root\n", argv[0]);
      exit(EXIT_FAILURE);
    }
    int ds, indice = 0;
    unsigned char ownMAC[6], ownIP[4], ownBroadcast[4], ownNetMask[4], trama_recibir[1514];
    struct ifreq Interfaz;
    struct ifreq * ptrInterfaz;
    char InterfazName[10];
    ds=abrirSocketRaw();
    if(getDefaultInterfaz(InterfazName) == -1)
      exit(EXIT_FAILURE);
    strcpy(Interfaz.ifr_name, InterfazName);
    indice=obtenerDatos(ds, &Interfaz, ownMAC, ownIP, ownBroadcast, ownNetMask);
    recibeTrama(ds, trama_recibir); 
    close (ds);
  }
  else if(argc == 2){
    FILE * archivotrama = NULL;
    if((archivotrama=fopen(*(argv+1), "r")) == NULL){
      perror("Error leyendo el archivo");
      exit(EXIT_FAILURE);
    }
    else{
      leerArchivoLLC(archivotrama);
      printf("\n===========================================================================================================\n");
    }
  }
  else
    usage(argv);
  return 0;
}