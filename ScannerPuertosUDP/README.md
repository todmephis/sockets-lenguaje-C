# Escaner de Puertos por UDP

### Información

Se programará un escaner de puertos UDP sencillo, hace un barrido desde el puerto 1 al 100 e indica si está abierto o no.

## User Datagram Protocol (UDP)

User Datagram Protocol (UDP) es un protocolo del nivel de transporte basado en el intercambio de datagramas (Encapsulado de capa 4 o de Transporte del Modelo OSI). Permite el envío de datagramas a través de la red sin que se haya establecido previamente una conexión, ya que el propio datagrama incorpora suficiente información de direccionamiento en su cabecera. Tampoco tiene confirmación ni control de flujo, por lo que los paquetes pueden adelantarse unos a otros; y tampoco se sabe si ha llegado correctamente, ya que no hay confirmación de entrega o recepción. Su uso principal es para protocolos como DHCP, BOOTP, DNS y demás protocolos en los que el intercambio de paquetes de la conexión/desconexión son mayores, o no son rentables con respecto a la información transmitida, así como para la transmisión de audio y vídeo en real, donde no es posible realizar retransmisiones por los estrictos requisitos de retardo que se tiene en estos casos.
[FUENTE](https://es.wikipedia.org/wiki/User_Datagram_Protocol)

##Funcionamiento
Enviamos un _segmento_ con la carga de datos de UDP en ceros, si el host remoto genera una respuesta, esta vendrá desde el protocolo ICMP con las el código 3,3 (Destination Unreachable, Destination port unreachable), esto quiere decir que el puerto se encuentra cerrado. Si el host remoto NO GENERA una respuesta significa que está abierto pero al no llevar los datos que espera, no genera respuesta alguna.


### ETHERNET HEADER
![ETHERNET HEADER](https://upload.wikimedia.org/wikipedia/commons/thumb/1/13/Ethernet_Type_II_Frame_format.svg/700px-Ethernet_Type_II_Frame_format.svg.png)
### IP HEADER
![IP HEADER](https://nmap.org/book/images/hdr/MJB-IP-Header-800x576.png)
### ICMP HEADER
![ICMP HEADER](https://nmap.org/book/images/hdr/MJB-ICMP-Header-800x392.png)
### UDP HEADER
![UDP HEADER](https://nmap.org/book/images/hdr/MJB-UDP-Header-800x264.png)


Este programa no requiere de recursos extras.

### man pages 
```
* No man pages recomendadas.
```

## Compilación

```
make scannerudp
```
**Ejecutar como usuario root (sudo).

### Comprobación
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ScannerPuertosUDP/caps/cap1.png )
![imagen 2](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ScannerPuertosUDP/caps/cap2.png )
### Instalación wireshark

```
sudo apt-get install wireshark
```
Ejecutar como root (sudo).

### ¿Preguntas?

Puedes hacerme llegar cualquiér opinión o duda que tengas. Dejaré mi contacto :)

## Autor

**Iván Sánchez**

*Contacto*

[Twitter](https://twitter.com/todmephis) 
[Telegram](http://telegram.me/todmephis)
[Facebook](https://www.facebook.com/0xSCRIPTKIDDIE1)



## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details

by todmephis