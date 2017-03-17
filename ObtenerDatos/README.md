# Obtener los datos de una interfaz de red

### Información

En este programa obtendremos los datos de una interfaz de red ingresada por el usuario desde stdin.

Los datos a obtener son:

```
* Índice de la interfaz de red seleccionada.
* Dirección física (MAC Address).
* Dirección lógica (IP Address).
* Dirección de broadcast.
* Máscara de red.
```

### Requerimentos

**Saber abrir un socket crudo**.

Este programa no requiere de recursos extras.

### man pages 
```
* man netdevice
* man ioctl
```

## Compilación

```
gcc C/obtenerdatos.c -o OUT/datos
```
**Ejecutar como usuario root (sudo).

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