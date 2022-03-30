# Informe práctica 2:

## 2.1a - Comunicación entre procesos

### Caso de comunicación entre procesos de la misma jerarquía:

Para realizar el ejercicio, el primer paso que se ha realizado es la creación del pipe a través de la función pipe, pasándole un array de 2 enteros como parmetro:

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define TAM 200
int hijo1, hijo2;
int tuberia[2];

////////////CASO DE COMUNICACION DE PROCESOS DE LA MISMA JERARQUIA////////////
int main(int argc, char const *argv[])
{
    pipe(tuberia);
    char mensaje[TAM];
    char mensaje1[TAM];
    hijo1 = fork();
```

### Código del primer hijo:

Tal y como se pide en el enunciado de la práctica, en el hijo 1 tiene que escribir el mensaje en el pipe, que será traducido a mayúsculas por el hijo 2:

```c
switch(hijo1)
    {
        case -1:
            perror("Error al crear el hijo 1");break;
        case 0:
            printf("\nÉxito al crear el hijo 1\n");
            close(tuberia[0]);//cerramos el lado de lectura
            printf("Introduce el mensaje a traducir: ");
            fflush(stdout);
            __fpurge(stdin);
            fgets(mensaje,200,stdin);
            write(tuberia[1], mensaje, strlen(mensaje));            
            break;
        default: hijo2 = fork();
```

Para realizar la escritura tendremos que cerrar el lado de lectura, luego enviaremos el mensaje introducido usando la función write:

```c
write(tuberia[1], mensaje, strlen(mensaje));
```

### Código del segundo hijo:

```c
case -1:
                perror("Error al crear el hijo 2");break;             
            case 0:
                printf("\nExito al crear el hijo 2\n");
                close(tuberia[1]);
                read(tuberia[0], mensaje1, TAM);
                printf("\n%s\n", mensaje1);

                for(int i = 0; i < TAM; i ++)
                {   
                    mensaje1[i] = toupper(mensaje1[i]);
                }
                printf("\n%s\n", mensaje1);
```

En este caso el hijo 2 para leer desde el pipe tendrá que cerrar el lado de escritura y leer utilizando la función read (guardando el mensaje en mensaje1):

```c
read(tuberia[0], mensaje1, TAM);
```

A continuación se procederá a traducir el mensaje y mostrarlo por pantalla en mayúsculas.

### Caso de comunicación entre procesos independientes (Cliente/Servidor)

**Named-Pipes:**

Tal y como se pide en la práctica se crean las colas a traves de las funciones **mkfifo()** y usando **write()** para la escritura:

```c
//Código cliente
/////ZONA DE ESCRITURA
    mkfifo("/tmp/fifo", 0666);
    fde = open("/tmp/fifo", O_WRONLY);
    write(fde, buf, sizeof(buf));
```

Para el caso de lectura se abre el fifo con **open()** y se lee con **read():**

```c
//Código cliente
/////ZONA DE LECTURA
fdl = open ("/tmp/fifo2", O_RDONLY);
n = read(fdl, traducido, sizeof(traducido));
printf("\nMensaje traducido: %s", traducido);
```

Por último es necesario cerrar el fifo con la función **close();**

El código del servidor es muy similar.

**Colas:**

Para la creación de una comunicación a través de colas, en primer lugar hay que establecer una cola:

```c
#define CLAVE 0x72357116L
```

Que será igual tanto en el lado del cliente como en el del servidor.

A continuación, en el lado del servidor se creará la cola de mensajes:

```c
//lado del servidor
idCola = msgget(CLAVE,IPC_CREAT|0666);
```

En el lado del cliente no será necesario crearlo:

```c
//lado del cliente
idCola = msgget(CLAVE,0666);
```

Tanto en el lado del cliente como en el lado del servidor será necesario crear una estructura que será la que contenga el mensaje que se desea enviar y además tendrá el tipo del canal (en formato long):

```c
typedef struct mensaje{
    long canal;
    char cad[200]; 
}MENSAJE;
```

Para la lectura de mensajes:

```c
msgrcv(idCola,&m,sizeof(m)-sizeof(long),0,0);
```

Para el envio de los mensajes:

```c
msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
```

Traducción en el lado del servidor:

 

```c
//codigo server
**msgrcv(idCola,&m,sizeof(m)-sizeof(long),1,0);
        for(int i=0;i<strlen(m.cad);i++){
            m.cad[i] = toupper(m.cad[i]);
        }**
```

## 2.1b.- Diseño de una aplicación (Cliente/Servidor) con múltiples
clientes

Par al realización del ejercicio se ha utilizado una estructura muy parecida al ejercicio anterior.

El código del servidor es el siguiente:

```c
//código servidor
#define CLAVE 0x72357116L

typedef struct mensaje{
    long canal;
    int pidCliente;
    char cad[100];
}MENSAJE;

int idCola;
MENSAJE m;

int main(int argc, char** argv) {

    idCola = msgget(CLAVE,IPC_CREAT|0666);
    
    while(1){
        msgrcv(idCola,&m,sizeof(m)-sizeof(long),1,0);
        printf("Cadena recibida: %s\n",m.cad);
        for(int i=0;i<strlen(m.cad);i++){
            m.cad[i] = toupper(m.cad[i]);
        }
        
        m.canal = m.pidCliente;
        msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
    }
    
    return (EXIT_SUCCESS);
```

Como se puede apreciar es prácticamente igual que en el ejercicio anterior, solo que en este caso para que diferencie mensajes de diferentes clientes se utiliza la siguiente linea para identificar cada proceso con su pid y enviar a través de ese canal:

```c
 m.canal = m.pidCliente;
```

Para la generación de cadenas aleatorias en el lado del cliente se ha utilizado el siguiente código:

```c
void generadorCadena(){
    char letras[] = "abcdefghijklmnopqrstuvwxyz";
    
    for(int i=0;i<((rand()%20)+1);i++){
        m.cad[i] = letras[rand()%(strlen(letras)-1)];
    }
    m.cad[strlen(m.cad)-1] = '\0';

}
```

```c
void aleatorio(){
    
    for(int i=0;i<(rand()%10)+1;i++){
        generadorCadena();
        m.canal = 1L;
        m.pidCliente = getpid();
        msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
        msgrcv(idCola,&m,sizeof(m)-sizeof(long),getpid(),0);
        printf("\nMensaje traducido: %s\n",m.cad);
    }
```

Si ejecutamos cliente y servidor obtendremos lo siguiente:

**Cliente**

![Captura de Pantalla 2022-03-30 a las 16.14.05.png](Informe%20pr%209a7b2/Captura_de_Pantalla_2022-03-30_a_las_16.14.05.png)

**Servidor**

![Captura de Pantalla 2022-03-30 a las 16.14.40.png](Informe%20pr%209a7b2/Captura_de_Pantalla_2022-03-30_a_las_16.14.40.png)

### Monitor

Resultado tras la correcta ejecución del monitor:

![Captura de Pantalla 2022-03-30 a las 10.48.28.png](Informe%20pr%209a7b2/Captura_de_Pantalla_2022-03-30_a_las_10.48.28.png)

Resultado tras la correcta ejecución del cliente:

![Captura de Pantalla 2022-03-30 a las 10.49.32.png](Informe%20pr%209a7b2/Captura_de_Pantalla_2022-03-30_a_las_10.49.32.png)

Para la realización del ejercicio de monitor se han reutilizado parte del código del apartado 2.1

**Ejercicio 1**

```c
void ejercicio1(void)
{
/////LECTURA///
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY);  
    n = read(fdl, buf, sizeof(buf));
    printf("\n mensaje recibido: %s\n", buf);

    close(fdl);
}
```

**Ejercicio 2**

```c
 void ejercicio2(void)
{
    printf("\nIntroduce el secreto 1:\n");
    fflush(stdin);    
    fgets(buf, TAM, stdin);
    __fpurge(stdout);
/////ZONA DE ESCRITURA///
    mkfifo("/tmp/fifo_monitor_2", 0666);
    fde = open("/tmp/fifo_monitor_2", O_WRONLY);
    write(fde, buf, sizeof(buf));
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY); 
    n = read(fdl, &buf1, sizeof(buf1));
    printf("\n Secreto 2: %s\n", buf1);
    
    
    write(fde, buf1, sizeof(buf1));
    
    close(fdl);
}
```

**Ejercicio 3**

```c
void ejercicio3(void){

    int idCola;

    MENSAJE m;

    idCola = msgget(CLAVE,0666);
    msgrcv(idCola,&m,sizeof(m)-sizeof(long),0,0);
    printf("Clave secreta 4: %s", m.cad);
    printf("Tipo de menasaje : <%ld> ", m.canal);
}
```

**Ejercicio 4 y 5**

```c
MENSAJE m2;

    ////////EJERCICIO 4//////////
    int idCola = msgget(CLAVE, IPC_CREAT|0666);  
    printf("\nIntroduce secretos(<333><444>): \n");
    fflush(stdin);    
    fgets(m2.cad, TAM, stdin);
    __fpurge(stdout);
    msgsnd(idCola,&m2,sizeof(m2)-sizeof(long),0);

    ////////EJERCICIO 5//////////
    printf("\nPulsa enter para comenzar el programa 5");
    getchar();
    msgrcv(idCola,&m2,sizeof(m2)-sizeof(long),getpid(),0);
    printf("\nSecreto 6: %s", m2.cad);
```

**Ejercicio 6 y 7**

```c
void ejercicio6_7()
{   
    /////EJERCICIO 6///////////////
    int idCola = msgget(CLAVE, IPC_CREAT|0666);
    MENSAJE m3;
    printf("\nPulsa enter para comenzar el programa 6");
    getchar();
    m3.canal = pidMonitor; 
    printf("\nIntroduce secreto 6 (<666>):\n");
    fflush(stdin);    
    fgets(m3.cad, TAM, stdin);  
    msgsnd(idCola,&m3,sizeof(m3)-sizeof(long),0);
    ////////EJERCICIO 7//////////
    printf("\nPulsa enter para comenzar el programa 7");
    getchar();
    msgsnd(idCola,&m3,sizeof(m3)-sizeof(long),0);

    msgctl(idCola,IPC_RMID,NULL);
    unlink("/tmp/fifo_monitor_1");
    unlink("/tmp/fifo_monitor_2");
    printf("\nPractica completada con éxito\n");    
}
```
