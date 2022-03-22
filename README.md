# ASI
Prácticas de la asignatura de ASI:


>## Tabla de contenidos:
- [P1](#p1)
- [P2: Comunicación entre procesos de la misma máquina](#p2)

## P1

## P2: Comunicación entre procesos de la misma máquina

### Descripcion
A la hora de intercambiar datos entre procesos que residen en la misma máquina, existen en UNIX una serie de recursos de comunicaciones clásicos que conviene que todo alumno conozca para poder utilizarlos en el diseño de aplicaciones más complejas. Entre estas soluciones de comunicaciones entre procesos (IPCs) se encuentran los pipes que es el sistema original de UNIX que permite comunicar dos procesos que son de la misma jerarquía, es decir que nacen del desdoble con fork() del mismo programa en ejecución. Muchas veces, necesitamos intercambiar datos entre procesos que no son de la misma jerarquía (es decir que ejecutan programas diferentes). Para ello es necesario disponer de un punto de encuentro común que nos permita identificar un recurso que puedan compartir ambos procesos para comunicarse. Es el caso de los named-pipes, que utilizan un fichero intermedio (en memoria) de intercambio de datos localizado en el sistema de ficheros, o el de las colas de mensajes del sistema, que propone un mecanismo de almacenamiento de mensajes por el sistema que puede ser utilizado eficazmente por los procesos para intercambiar información. Esta serie de recursos son el objeto de trabajo de esta práctica. Hay en internet infinidad de ejemplos para su uso. Conociendo estos recursos se pueden resolver problemas prácticos de comunicaciones entre procesos dentro de una misma máquina. En la segunda parte de la práctica se propone al alumno que diseñe una aplicación de comunicaciones de mensajes un poco más completa.
