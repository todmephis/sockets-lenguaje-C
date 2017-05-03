# Programando un PING

### Información

Se programará una utilidad similar al ping de los sistemas operativos con el objetivo de entender los protocolos ICMP e IP y analizar las solicitudes de _ECHO_.

##¿Qué es el PING?
Como programa, ping es una utilidad diagnóstica1 en redes de computadoras que comprueba el estado de la comunicación del host local con uno o varios equipos remotos de una red IP por medio del envío de paquetes ICMP de solicitud (ICMP Echo Request) y de respuesta (ICMP Echo Reply). Mediante esta utilidad puede diagnosticarse el estado, velocidad y calidad de una red determinada.3

Ejecutando Ping de solicitud, el Host local envía un mensaje ICMP, incrustado en un paquete IP. El mensaje ICMP de solicitud incluye, además del tipo de mensaje y el código del mismo, un número identificador y una secuencia de números, de 32 bits, que deberán coincidir con el mensaje ICMP de respuesta; además de un espacio opcional para datos. Como protocolo ICMP no se basa en un protocolo de capa de transporte como TCP o UDP y no utiliza ningún protocolo de capa de aplicación.

Muchas veces se utiliza para medir la latencia o tiempo que tardan en comunicarse dos puntos remotos, y por ello, se utiliza el término PING para referirse al lag o latencia de la conexión en los juegos en red.

[FUENTE](https://es.wikipedia.org/wiki/Ping)

### Utilidad PING
![PING](https://image.ibb.co/f5HT35/Screenshot_from_2017_05_02_19_51_17.png)

### ETHERNET HEADER
![ETHERNET HEADER](https://upload.wikimedia.org/wikipedia/commons/thumb/1/13/Ethernet_Type_II_Frame_format.svg/700px-Ethernet_Type_II_Frame_format.svg.png)
### IP HEADER
![IP HEADER](https://nmap.org/book/images/hdr/MJB-IP-Header-800x576.png)
### ICMP HEADER
![ICMP HEADER](https://nmap.org/book/images/hdr/MJB-ICMP-Header-800x392.png)




### Requerimentos

**Entender los programas anteriores de manera general (Enviar y recibir tramas)**
**Haber enviado y recibido tramas de ARP**
**Conocer los _headers_ de Ethernet, ARP, IP e ICMP**

Este programa no requiere de recursos extras.

### man pages 
```
* No man pages recomendadas.
```

## Compilación

```
make ping
```
**Ejecutar como usuario root (sudo).

### Comprobación
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Ping/caps/ping1.png "Ping ipn.mx")
![imagen 2](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Ping/caps/ping2.png "Paquetes en Wireshark")
![imagen 3](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Ping/caps/ping3.png "Ping 192.168.200.1 (Gateway)")
![imagen 4](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Ping/caps/ping4.png "Paquetes en Wireshark")

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