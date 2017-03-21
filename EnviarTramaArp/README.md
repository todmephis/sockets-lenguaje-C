# Enviar una petición de ARP

### Información

Envia una petición ARP a una dirección IP ingresada desde stdin.

## ¿Qué es ARP?

En red de computadoras, el protocolo de resolución de direcciones (ARP, del inglés Address Resolution Protocol) es un protocolo de comunicaciones de la capa de enlace, responsable de encontrar la dirección de hardware (Ethernet MAC) que corresponde a una determinada dirección IP. Para ello se envía un paquete (ARP request) a la dirección de difusión de la red (broadcast, MAC = FF FF FF FF FF FF) que contiene la dirección IP por la que se pregunta, y se espera a que esa máquina (u otra) responda (ARP reply) con la dirección Ethernet que le corresponde.

Fuente: [Wikipedia](https://es.wikipedia.org/wiki/Protocolo_de_resoluci%C3%B3n_de_direcciones)

## Estructura de una trama ARP.
![ARP](https://redesus2.wikispaces.com/file/view/Paquete_ARP.jpg/101290301/Paquete_ARP.jpg)

### Requerimentos

**Haber enviado y recibido tramas arbitrarias satisfactoriamente*.

Este programa no requiere de recursos extras.

### man pages 
```
* man gettimeofday
* man inet
* man 7 ip
```

## Compilación

```
gcc C/enviararp.c -o OUT/enviararp
```
**Ejecutar como usuario root (sudo).

### Comprobación
Para comprovar el envío de del paquete podemos utilizar el sniffer de red "Wireshark".
El resultado es algo como lo siguiente:
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/EnviarTramaArp/caps/enviaArp.png "TRAMA ENVIADA Y RECIBIDA")

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