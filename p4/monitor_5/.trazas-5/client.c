#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TAMCAD 20

struct sockaddr_in servidor,cliente;
int ds,dc,n,tam,sec2,num;
char cad[TAMCAD];

void esperar(){
    wait(NULL);
}

int main(int argc, char** argv) {

    // Ejercicio 1
    // Inicializaremos las variables para hacer del cliente TCP
    cliente.sin_family = AF_INET;
    cliente.sin_addr.s_addr = htonl(INADDR_ANY);
    cliente.sin_port = htons(3000);
    bzero(&(cliente.sin_zero),8);
    
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons(3000);
    bzero(&(servidor.sin_zero),8);
    
    // Crearemos el socket indicando con el parametro SOCK_STREAM que estamos
    // usando el protocolo TCP
    ds = socket(AF_INET,SOCK_STREAM,0);
    
    // Nos conectaremos al socket
    connect(ds,(struct sockaddr*)&servidor,sizeof(servidor));
    
    // Leeremos el secreto 2 que nos envia el servidor (el monitor)
    n = read(ds,cad,TAMCAD);
    // Añadiremos el \0 a la cadena que hemos recibido
    cad[n] = '\0';
    
    // Extraeremos el numero entero que contiene la cadena ya que este 
    // está en formato binario
    sec2 = *((int*)cad);
    printf("Secreto 2: %d\n",sec2);
    
    // Formatearemos el mensaje de la forma en la que nos piden en el enunciado
    sprintf(cad,"<%d>",sec2);
    
    // Enviaremos el secreto 2 al servidor (el monitor)
    write(ds,cad,strlen(cad));
    
    // Cerraremos la conexión TCP
    close(ds);
    
    printf("Fin del ejercicio 1\n");
    getchar();
    
    
    // Ejercicio 2
    // Inicializaremos las variables correspondientes para hacer de servidor TCP
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons(3001);
    bzero(&(servidor.sin_zero),8);
    
    // Crearemos el socket indicando que vamos a usar el protocolo TCP
    ds = socket(AF_INET,SOCK_STREAM,0);
    // Publicaremos los datos del servidor
    bind(ds,(struct sockaddr*)&servidor,sizeof(servidor));
    
    // Estableceremos el numero maximo de clientes que se podrán conectar a
    // nuestro socket. En este caso usaremos la constante SOMAXCONN que indica
    // el numero maximo de clientes permitidos para un socket
    listen(ds,SOMAXCONN);
    // Haremos que el proceso padre use la funcion esperar cuando reciba una
    // señal SIGCHLD (Esta señal la envian los hijos cuando han terminado 
    // su ejecución
    signal(SIGCHLD,esperar);
    // Crearemos un hijo que se encargara de aceptar las conexiones TCP
    // y de crear hijos para hacer las tareas de eco
    if(fork()==0){
        // Haremos que el servidor escuche y responda a conexiones TCP 
        while(1){
            tam = sizeof(cliente);
            // Aceptaremos las conexiones TCP que nos hagan los clientes
            dc = accept(ds,(struct sockaddr*)&cliente,&tam);
            // Crearemos los hijos que se encargarán de las tareas de eco
            if(fork()==0){
                // Cerramos el descriptor del servidor ya que el hijo no lo necesitará
                close(ds);
                // Bucle que se ejecuta mientras el cliente nos esté enviando datos
                while(read(dc,&num,sizeof(int))==sizeof(int)){
                    // Formatearemos la cadena en la forma en la que nos piden en el enunciado
                    sprintf(cad,"<%d>",num);
                    // Enviaremos la caden al cliente
                    write(dc,cad,strlen(cad));
                }
                // Cerraremos el descriptor del servidor
                close(dc);
                // Haremos que los hijos salgan con un codigo 0 (Todo correcto)
                exit(0);
            }else{
                // El hijo que es el padre de todos los procesos que crea
                // cerrará el descriptor del servidor
                close(dc);
            }
        }
    }
    
    printf("Fin del ejercicio 2\n");
    getchar();
    
    
    // Ejercicio 3
    // Inicializaremos las variables para hacer de cliente UDP
    servidor.sin_port = htons(3000);
    servidor.sin_family = AF_INET;
    bzero(&(servidor.sin_zero),8);
    
    cliente.sin_addr.s_addr = htonl(INADDR_ANY);
    cliente.sin_port = htons(3000);
    cliente.sin_family = AF_INET;
    bzero(&(cliente.sin_zero),8);
    
    // Crearemos el socket indicando con el parametro SOCK_DGRAM
    // que vamos a usar el protocolo UDP
    ds = socket(AF_INET,SOCK_DGRAM,0);
    // Publicaremos los datos del cliente
    bind(ds,(struct sockaddr*)&cliente,sizeof(cliente));

    tam = sizeof(cliente);
    // Crearemos la cadena <???>
    sprintf(cad,"<???>");
    // Enviaremos la cadena correspondiente al servidor UDP (el monitor)
    sendto(ds,cad,strlen(cad),0,(struct sockaddr*)&servidor,tam);
    // Recibiremos el secreto 8 por parte del servidor (el monitor)
    recvfrom(ds,cad,strlen(cad),0,(struct sockaddr*)&servidor,&tam);
    printf("Secreto 8: %s\n",cad);
    // Leeremos el entero correspondiente al secreto 8 de la cadena con un 
    // formato determinado
    sscanf(cad,"<%d>",&num);
    // Enviaremos de vuelta el secreto 8 al servidor UDP (el monitor)
    sendto(ds,&num,sizeof(int),0,(struct sockaddr*)&servidor,tam);
    
    printf("Fin del ejercicio 3\n");
    getchar();

    
    // Ejercicio 4
    
    // Inicializaremos las variables para hacer de servidor UDP
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons(3001);
    servidor.sin_family = AF_INET;
    bzero(&(servidor.sin_zero),8);
    
    // Crearemos el socket
    ds = socket(AF_INET,SOCK_DGRAM,0);
    
    // Publicaremos los datos del servidor
    bind(ds,(struct sockaddr*)&servidor,sizeof(servidor));
    
    // Haremos que el servidor UDP reciba mensajes de diferentes clientes
    // de forma concurrente
    while(1){
        tam = sizeof(cliente);
        // Recibiremos el secreto 10 que nos envia el cliente (el monitor)
        recvfrom(ds,&num,sizeof(int),0,(struct sockaddr*)&cliente,&tam);
        // Crearemos un hijo para que responda a los mensajes
        if(fork()==0){
            // Formatearemos la cadena de la forma en la que nos la piden
            // en el enunciado
            sprintf(cad,"<%d>",num);
            // Enviaremos de vuelta al cliente (el monitor) el mensaje que
            // nos ha enviado
            sendto(ds,cad,strlen(cad),0,(struct sockaddr*)&cliente,tam);
        }
    }
    
    printf("Fin del ejercicio 4\n");
    getchar();
    
    return (EXIT_SUCCESS);
}
