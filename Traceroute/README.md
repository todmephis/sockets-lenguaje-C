# Programando Traceroute

### Información

Se programará una utilidad similar al traceroute de los sistemas operativos con el objetivo de entender los protocolos ICMP e IP y analizar las solicitudes de _ECHO_.

##¿Qué es el Traceroute?
Traceroute es una consola de diagnóstico que permite seguir la pista de los paquetes que vienen desde un host (punto de red). Se obtiene además una estadística del RTT o latencia de red de esos paquetes, lo que viene a ser una estimación de la distancia a la que están los extremos de la comunicación. Esta herramienta se llama traceroute en UNIX, Mac1 y GNU/Linux, mientras que en Windows se llama tracert.

##Funcionamiento
El número de la primera columna es el número de salto, posteriormente viene el nombre y la dirección IP del nodo por el que pasa, los tres tiempos siguientes son el tiempo de respuesta para los paquetes enviados (un asterisco indica que no se obtuvo respuesta).

Estas herramientas (traceroute y tracert) son órdenes ejecutables en una consola en modo texto.

Tracert utiliza el campo Time To Live (TTL) de la cabecera IP. Este campo sirve para que un paquete no permanezca en la red de forma indefinida (por ejemplo, debido a la existencia en la red de un bucle cerrado en la ruta). El campo TTL es un número entero que es decrementado por cada nodo por el que pasa el paquete. De esta forma, cuando el campo TTL llega al valor 0 ya no se reenviará más, sino que el nodo que lo esté manejando en ese momento lo descartará. Lo que hace tracert es mandar paquetes a la red de forma que el primer paquete lleve un valor TTL=1, el segundo un TTL=2, etc. De esta forma, el primer paquete será eliminado por el primer nodo al que llegue (ya que éste nodo decrementará el valor TTL, llegando a cero). Cuando un nodo elimina un paquete, envía al emisor un mensaje de control especial indicando una incidencia. Tracert usa esta respuesta para averiguar la dirección IP del nodo que desechó el paquete, que será el primer nodo de la red. La segunda vez que se manda un paquete, el TTL vale 2, por lo que pasará el primer nodo y llegará al segundo, donde será descartado, devolviendo de nuevo un mensaje de control. Esto se hace de forma sucesiva hasta que el paquete llega a su destino.


[FUENTE](https://es.wikipedia.org/wiki/Traceroute)

### Utilidad TRACEROUTE
![TRACEROUTE](http://1.bp.blogspot.com/-eKSgmqud0ls/UTKrL93BkHI/AAAAAAAAADU/H2aTTe-rjvY/s1600/Screen+Shot+2013-03-03+at+9.42.09+AM.png)

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
**Haber programado el PING**

Este programa no requiere de recursos extras.

### man pages 
```
* No man pages recomendadas.
```

## Compilación

```
make traceroute
```
**Ejecutar como usuario root (sudo).

### Comprobación
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Traceroute/caps/traceroute1.png "Traceroute google.com")
![imagen 2](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Traceroute/caps/traceroute2.png "Paquetes en Wireshark")
![imagen 3](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Traceroute/caps/traceroute3.png "Traceroute 192.168.200.1 (Gateway)")
![imagen 4](https://github.com/todmephis/sockets-lenguaje-C/blob/master/Traceroute/caps/traceroute4.png "Paquetes en Wireshark")

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