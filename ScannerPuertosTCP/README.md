# Escaner de Puertos por TCP

### Información

Se programará un escaner de puertos TCP sencillo, hace un barrido desde el puerto 1 al 100 e indica si está abierto o no.

## Transmission Control Protocol (TCP)

La descripción completa del protocolo TCP se encuentra en el documento RFC793 o su traducción al español. TCP (Transmission-Control-Protocol, en español Protocolo de Control de Transmisión) es de los protocolos fundamentales en Internet. Fue creado entre los años 1973 - 1974 por Vint Cerf y Robert Kahn.

Muchos programas dentro de una red de datos compuesta por computadoras pueden usar TCP para crear conexiones entre ellos a través de las cuales puede enviarse un flujo de datos. El protocolo garantiza que los datos serán entregados en su destino sin errores y en el mismo orden en que se transmitieron. También proporciona un mecanismo para distinguir distintas aplicaciones dentro de una misma máquina, a través del concepto de puerto.

TCP da soporte a muchas de las aplicaciones más populares de Internet, incluidas HTTP, SMTP, SSH y FTP.

TCP es un protocolo de comunicación orientado a conexión y fiable del nivel de transporte, actualmente documentado por IETF en el RFC 793. Es un protocolo de capa 4 según el modelo OSI.

Funciones de TCP
En la pila de protocolos TCP/IP, TCP es la capa intermedia entre el protocolo de internet (IP) y la aplicación. Habitualmente, las aplicaciones necesitan que la comunicación sea fiable y, dado que la capa IP aporta un servicio de datagramas no fiable (sin confirmación), TCP añade las funciones necesarias para prestar un servicio que permita que la comunicación entre dos sistemas se efectúe libre de errores, sin pérdidas y con seguridad.

Los servicios provistos por TCP corren en el anfitrión (host) de cualquiera de los extremos de una conexión, no en la red. Por lo tanto, TCP es un protocolo para manejar conexiones de extremo a extremo. Tales conexiones pueden existir a través de una serie de conexiones punto a punto, por lo que estas conexiones extremo-extremo son llamadas circuitos virtuales. Las características del TCP son:

Orientado a la conexión: dos computadoras establecen una conexión para intercambiar datos. Los sistemas de los extremos se sincronizan con el otro para manejar el flujo de paquetes y adaptarse a la congestión de la red.
Operación Full-Duplex: una conexión TCP es un par de circuitos virtuales, cada uno en una dirección. Sólo los dos sistemas finales sincronizados pueden usar la conexión.
Error Checking: una técnica de checksum es usada para verificar que los paquetes no estén corruptos.
Acknowledgements: sobre recibo de uno o más paquetes, el receptor regresa un acknowledgement (reconocimiento) al transmisor indicando que recibió los paquetes. Si los paquetes no son notificados, el transmisor puede reenviar los paquetes o terminar la conexión si el transmisor cree que el receptor no está más en la conexión.
Flow Control: si el transmisor está desbordando el buffer del receptor por transmitir demasiado rápido, el receptor descarta paquetes. Los acknowledgement fallidos que llegan al transmisor le alertan para bajar la tasa de transferencia o dejar de transmitir.
Servicio de recuperación de Paquetes: el receptor puede pedir la retransmisión de un paquete. Si el paquete no es notificado como recibido (ACK), el transmisor envía de nuevo el paquete.
Los servicios confiables de entrega de datos son críticos para aplicaciones tales como transferencias de archivos (FTP por ejemplo), servicios de bases de datos, proceso de transacciones y otras aplicaciones de misión crítica en las cuales la entrega de cada paquete debe ser garantizada.
[FUENTE](https://es.wikibooks.org/wiki/Redes_inform%C3%A1ticas/Protocolos_TCP_y_UDP_en_el_nivel_de_transporte)

##Funcionamiento
Enviamos un _segmento_ con el puerto destino a probar y con la bandera _SYN_ encendida. Si el puerto que solicitamos está abierto tendremos una respuesta desde ese puerto solicitado y la bandera _SYN_ y _ACK_ encendida (establecimiento de conexión positiva). Si el puerto se encuentra cerrado tendremos esa misma respuesta pero esta vez con la bandera _RST_ y _ACK_ encendida, (establecimiento de conexión negativa)
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ScannerPuertosTCP/caps/cap1.png )
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ScannerPuertosTCP/caps/cap2.png )



### ETHERNET HEADER
![ETHERNET HEADER](https://upload.wikimedia.org/wikipedia/commons/thumb/1/13/Ethernet_Type_II_Frame_format.svg/700px-Ethernet_Type_II_Frame_format.svg.png)
### IP HEADER
![IP HEADER](https://nmap.org/book/images/hdr/MJB-IP-Header-800x576.png)
### ICMP HEADER
![ICMP HEADER](https://nmap.org/book/images/hdr/MJB-ICMP-Header-800x392.png)
### TCP HEADER
![TCP HEADER](https://nmap.org/book/images/hdr/MJB-TCP-Header-800x564.png)


Este programa no requiere de recursos extras.

### man pages 
```
* No man pages recomendadas.
```

## Compilación

```
make scannertcp
```
**Ejecutar como usuario root (sudo).

### Comprobación
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ScannerPuertosTCP/caps/cap3.png )
![imagen 2](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ScannerPuertosTCP/caps/cap4.png )
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