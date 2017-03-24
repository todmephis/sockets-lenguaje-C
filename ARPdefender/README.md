# ARP defender (servidorARP)

### Información

ARP defender (servidorARP) impide que un host tome la dirección IP de algún otro host que ya esté registrado en la base de datos que poblamos anteriormente con las direcciones físicas y lógicas con el [analizador ARP](https://github.com/todmephis/sockets-lenguaje-C/tree/master/AnalizadorArp)..

### ¿Cómo funciona?

Pongamos el siguiente ecenario:

En una pequeña empresa el encargado de la red ha colocado direcciones IP de manera manual repartidas en todos los departamentos de dicha empressa. Como es de costumbre, hay restricciones que el "jefe" (o dueño) de la empresa quiere que se cumplan como por ejemplo. En un horario de 9:00 hasta las 18:00 los host no pueden accesar a sitios como Facebook, Netflix, Xvideos, etc. Entre otras restricciones particulares. 

El jefe decide que su dirección IP no tendrá ninguna restricción y que en contraste tendrá algunos otros privilegios. Todo funciona de maravilla pero tiene un becario que se percata que mientras ellos están restringidos la dirección del jefe es privilegiada. 

La dirección IP del jefe es la 192.168.1.20 y su dirección MAC es aa:aa:aa:aa:aa:aa.
El becario intenta tomar esta misma dirección IP con la dirección MAC cc:cc:cc:cc:cc:cc.

Si la computadora del jefe está encedida y el becario intenta tomar la direcció IP; la computadora del jefe de manera automática le contestará a su petición ARP gratuitous impidiendo la conexión del becario. 
Si la computadora del jefe está apagada entonces la petición ARP gratuitous del becario no tendrá respuesta y tomará la dirección IP de manera correcta.

Este programa entra en el segundo caso (computadora apagada) y contestará la petición ARP gratuitous del becario impidiendo su conexión y de esta manera proteger la IP del jefe aun que su computadora se encuentre apgada.

### ARP gratuitous

Para que sirve ?
Cada vez que un ordenador inicia la configuración de una interfaz de red que hace uso del protocolo ARP, el protocolo ARP envía un paquete ARP con el fin de determinar si su dirección IP o dirección física está siendo utilizada por otro computador y así ofrecer la oportunidad a los demás computadores de actualizar su ARP Cache.
En un paquete ARP gratuito la dirección fisica destino es igual a la dirección física Fuente, y la dirección IP destino es igual a la dirección IP fuente. Si algún computador de la red local detecta un paquete ARP gratuito cuya dirección IP es la misma que la configurada en su interface de red el mismo envía un paquete ARP indicando su dirección IP y dirección física al computador que transmitió el ARP gratuito. De esta manera el computador que envió el ARP gratuito genera un mensaje de advertencia indicando que dicha dirección IP está siendo utilizada por otro computador.
Por lo tanto una máquina puede enviar una solicitud ARP preguntando sobre su propia dirección IP. 

Propósitos:

* Detectar direcciones IP duplicadas.
* Forzar a que todos actualicen la entrada de la cache correspondiente.

[Fuente](http://laura14792.blogspot.mx/2013/12/arp-gratuito.html)


### Requerimentos

**Haber realizado satisfactoriamente el [analizador ARP](https://github.com/todmephis/sockets-lenguaje-C/tree/master/AnalizadorArp).**

Este programa REQUIERE de los archivos headers de MySQL C connector.
Puedes instalarlo con el siguiente comando:
```
sudo apt-get install libmysqlclient-dev
```
O puedes bajarlos directamente del [sitio web](https://dev.mysql.com/downloads/connector/c/)

### man pages 
```
* No se requiere leer otros manuales.
```
[MySQL functions](https://dev.mysql.com/doc/refman/5.7/en/c-api-functions.html)

## Compilación

```
gcc C/ARPdefender.c -o OUT/servidor -I /usr/include/mysql/ -lmysqlclient

```
**Ejecutar como usuario root (sudo).

### Comprobación


Ingresé de forma manual una dirección IP y un MAC asociadas. (192.168.1.250 y aa:aa:aa:aa:aa:aa).
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def1.png)

La configuración actual del host con Windows es la siguiente (su dirección MAC e IP están registradas en la BDD).
![imagen 2](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def2.png)

Solicitaré la IP 192.168.1.250 que originalmente pertenece a aa:aa:aa:aa:aa:aa
![imagen 3](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def3.png)

Acepto los cambios y la respuesta en el server es la siguiente
![imagen 4](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def4.png)

Los paquetes en Wireshark
![imagen 5](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def5.png)

Mientras tanto en el host con Windows, el ping a 8.8.8.8 ya no responde (no tiene Internet) y nos notifica un conflicto con las direcciones IP.
![imagen 6](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def6.png)
![imagen 7](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def7.png)

Regreso a su configuración real y la respuesta en el server es la siguiente
![imagen 8](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def8.png)

Mientras que en mi host el ping ya responde
![imagen 9](https://github.com/todmephis/sockets-lenguaje-C/blob/master/ARPdefender/caps/def9.png)

### Instalación wireshark

```
sudo apt-get install wireshark
```
**Ejecutar como root (sudo).

## To do list
- [ ] Implementar un log.
- [ ] Manejar una dirección IP asociada a más de una dirección MAC.

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