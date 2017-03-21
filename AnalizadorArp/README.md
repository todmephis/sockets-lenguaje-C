# Analizador ARP

### Información

Escanea una red (por el momento solo tipo C) enviando una petición de ARP a todas las direcciones lógicas posibles en esa red.
Los host que contesten a la petición de ARP será almacenada su dirección IP y su dirección MAC en una base de datos en MySQL para su futuro uso. 


### Requerimentos

**Haber realizado satisfactoriamente el programa que envía peticiones ARP a una dirección IP ingresada por el usuario desde stdin.**

Este programa REQUIERE de los archivos headers de MySQL C connector.
Puedes instalarlo con el siguiente comando:
```
sudo apt-get install libmysqlclient-dev
```
O puedes bajarlos directamente del [sitio web](https://dev.mysql.com/downloads/connector/c/)

### man pages 
```
* man termios
* man gettimeofday
* [MySQL functions](https://dev.mysql.com/doc/refman/5.7/en/c-api-functions.html)
```

## Compilación

```
gcc C/analizadorarp.c -o OUT/analizador -I /usr/include/mysql/ -lmysqlclient

```
**Ejecutar como usuario root (sudo).

### Comprobación
Para comprobar el envío de del paquete podemos utilizar el sniffer de red "Wireshark".

El resultado es algo como lo siguiente:
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/AnalizadorArp/caps/analizadorarp1.png)
![imagen 2](https://github.com/todmephis/sockets-lenguaje-C/blob/master/AnalizadorArp/caps/analizadorarp2.png)
![imagen 3](https://github.com/todmephis/sockets-lenguaje-C/blob/master/AnalizadorArp/caps/analizadorarp3.png)
![imagen 4](https://github.com/todmephis/sockets-lenguaje-C/blob/master/AnalizadorArp/caps/analizadorarp4.png)
![imagen 5](https://github.com/todmephis/sockets-lenguaje-C/blob/master/AnalizadorArp/caps/analizadorarp5.png)

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