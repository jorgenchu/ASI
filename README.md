# ASI
Prácticas de la asignatura de ASI:


>## Tabla de contenidos:
- [P1: Procesos y señales](#p1)
- [P2: Comunicación entre procesos de la misma máquina](#p2)

## P1: Procesos y señales
Crea un programa que al ejecutarse se desdoble el proceso padre en otros dos procesos hijos. El proceso padre se quedará dormido e igualmente ocurrirá con el primer hijo. El segundo hijo estará pendiente del teclado (proceso de teclado) esperando recoger una orden del operador que le indique si envía una señal al proceso padre, si lo hace al hijo o si finaliza la comunicación. Cada proceso mandará un mensaje a la pantalla al recibir la señal del proceso de teclado y continuará pendiente de recibir más señales.
Por otro lado, el proceso de teclado debe tener una opción para construir un mensaje en un fichero llamado “mensaje” que será traducido de minúsculas a mayúsculas por un thread del proceso hijo primero. La traducción se realizará cuando el proceso teclado envíe una señal al proceso hijo primero que le dé a entender que el mensaje está ya disponible para su traducción. El resultado de la traducción aparecerá en pantalla informando el thread de qué proceso lo ha realizado.
En el menú del proceso de teclado se contemplará una opción para cerrar el programa de una forma ordenada.


## P2: Comunicación entre procesos de la misma máquina

### Descripcion
A la hora de intercambiar datos entre procesos que residen en la misma máquina, existen en UNIX una serie de recursos de comunicaciones clásicos que conviene que todo alumno conozca para poder utilizarlos en el diseño de aplicaciones más complejas. Entre estas soluciones de comunicaciones entre procesos (IPCs) se encuentran los pipes que es el sistema original de UNIX que permite comunicar dos procesos que son de la misma jerarquía, es decir que nacen del desdoble con fork() del mismo programa en ejecución. Muchas veces, necesitamos intercambiar datos entre procesos que no son de la misma jerarquía (es decir que ejecutan programas diferentes). Para ello es necesario disponer de un punto de encuentro común que nos permita identificar un recurso que puedan compartir ambos procesos para comunicarse. Es el caso de los named-pipes, que utilizan un fichero intermedio (en memoria) de intercambio de datos localizado en el sistema de ficheros, o el de las colas de mensajes del sistema, que propone un mecanismo de almacenamiento de mensajes por el sistema que puede ser utilizado eficazmente por los procesos para intercambiar información. Esta serie de recursos son el objeto de trabajo de esta práctica. Hay en internet infinidad de ejemplos para su uso. Conociendo estos recursos se pueden resolver problemas prácticos de comunicaciones entre procesos dentro de una misma máquina. En la segunda parte de la práctica se propone al alumno que diseñe una aplicación de comunicaciones de mensajes un poco más completa.
