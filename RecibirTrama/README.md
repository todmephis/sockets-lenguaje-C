# Recibir una trama filtrando el tráfico.

### Información

Recibiremos todo el tráfico en la red y filtraremos para recibir la trama enviada por el programa anterior.
Los filtros serán de acuerdo a las necesidades y objetivos del programa.


### Requerimentos

**Haber obtenido los datos de la interfaz de Red satisfactoriamente**.

Este programa no requiere de recursos extras.

### man pages 
```
* man recv
* man 7 packet
```

## Compilación

```
gcc C/recibirtrama.c -o OUT/recibe
```
**Ejecutar como usuario root (sudo).

### Comprobación
Para comprobar el envío de del paquete podemos utilizar el sniffer de red "Wireshark".
El resultado es algo como lo siguiente:
![imagen 1](https://github.com/todmephis/sockets-lenguaje-C/blob/master/RecibirTrama/caps/recibirtrama.png "Trama enviada y recibida por el programa recien hecho y comprobado el paquete en Wireshark.")

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