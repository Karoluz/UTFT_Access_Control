# UTFT_Access_Control
Aplicacion desarrollada para control de acceso, usando una interface touch UTFT 320QVT y Arduino mega.

Proceso de Instalacion.
- Instalar Arduino IDE.
- Instalar las librerias del display
- Descargar en el siguiente link:http://www.rinkydinkelectronics.com/library.php?id=92
- Instalar las librerias desde el administrador de librerias en el IDE de arduino o descomprimirlas y copiarlas manualmente.

Esta aplicacion es un ejemplo de uso para el touchscreen.

Ventana de inicio
![UTFTAppDigit](https://github.com/Karoluz/UTFT_Access_Control/blob/master/1701739911941.jpg)

Su funcion principal es desplegar en pantalla una interfaz para el control de acceso.
muestra un teclado numerico en el cual puedes acceder presionando el touch, o para pruebas se agrego la comunicacion serial.
puedes enviar los numero del 0-9 para seleccionar cuatro digitos, y otros botones de accion, para eliminar un digito, para limpiar todo los digitos, y para entrar.
![UTFTAppInit](https://github.com/Karoluz/UTFT_Access_Control/blob/master/1701739911920.jpg)

Al tener los cuatro digitos correctos y presionar enter, cambiara a una ventana nueva donde se tendra el acceso a la aplicacion.
![UTFTAppPass](https://github.com/Karoluz/UTFT_Access_Control/blob/master/1701739911900.jpg)
