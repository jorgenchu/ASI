#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h> // para definir la estructura sockaddr_in

#define PORT 3000
#define PORT1 3001
#define BUF_SIZE 100
/////DEFINICION VARIABLES SOCKET//////
struct sockaddr_in cliente, servidor;
int socketfd, con, n, li, bin, new_socketfd, tam;

///DEFINICION DE VARIABLES//
int len_rx, len_tx = 0;
char buff_tx[BUF_SIZE];
char buff_rx[BUF_SIZE];
char secreto2[BUF_SIZE];
///////////////////////////


int main(int argc, char const *argv[])
{   
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1){
        perror("\nError al crear el socket\n");
        return -1;
    }
    printf("\nSOCKET CREADO");
////////////////DECLARACION DEL cliente///////////////////
    cliente.sin_family = AF_INET;
    cliente.sin_port = htons(PORT);
    cliente.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(cliente.sin_zero),8);
///////////////////////////////////////////////////////////

//////////////////////CONEXIÓN/////////////////////////////
    printf("\nEsperando al secreto 2 para realizar la conexión\n");
    con = connect(socketfd, (struct sockaddr *) &cliente, sizeof(cliente));
    if(con==-1){
        perror("\nError al crear al conectar\n");
        return -1;
    }
    printf("\nConexión realizada con éxito\n");

////////////////////////EJERCICIO 2////////////////////////
    read(socketfd,buff_rx, sizeof(buff_rx));
    strcpy(secreto2, buff_rx);
    printf("\nEl secreto 2 es: %s\n", secreto2);
    
////////////////////////EJERCICIO 3////////////////////////
    printf("\nEjecutando Ejercicio 3");
    strcpy(buff_tx, secreto2);
    write(socketfd,buff_tx, sizeof(buff_tx));
    //recepcion y reenvio del mensaje
    n = read(socketfd,buff_rx, sizeof(buff_rx));
    buff_rx[n] ='\0';
    write(socketfd,buff_rx, sizeof(buff_rx));
    printf("\nMensaje = %s\n", buff_rx);
    close(socketfd);
    
////////////////////////EJERCICIO 4////////////////////////
    printf("\nEjecutando Ejercicio 4");
    //Declaración del servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORT1);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(servidor.sin_zero),8);
    
    //creacion del socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1){
        perror("\nError al crear el socket\n");
        return -1;
    }

    bin = bind(socketfd, (struct sockaddr *) &servidor, sizeof(servidor));
    if(bin == -1){
        perror("\nError al binnear\n");
        return -1;
    }
    printf("\n[SERVIDOR]: BIND OK");
    li = listen(socketfd, SOMAXCONN);
    if(li == -1){
        perror("\nError al escuchar\n");
        return -1;
    }
    printf("\n[SERVIDOR]: LISTEN OK\n");
    tam = sizeof(cliente);
    new_socketfd = accept(socketfd, (struct sockaddr *) &cliente, &tam);
      if(new_socketfd == -1){
        perror("\nError al aceptar\n");
        return -1;
    }
    printf("\n[SERVIDOR]: CONEXION ACEPTADA\n");

////////////////////EJERCICIO 5//////////////////////////////
    printf("\nEjecutando Ejercicio 5");
    n = read(new_socketfd,buff_rx, sizeof(buff_rx)); 
    buff_rx[n] ='\0';
    write(new_socketfd,buff_rx, sizeof(buff_rx));
    printf("\nMensaje enviado con éxito\n");
    close(socketfd);

////////////////////EJECICIO 6//////////////////////////////////
    printf("\nEjecutando Ejercicio 6\nPress Enter to continue\n");
    getchar();
    servidor.sin_port = htons(PORT);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    bin = bind(socketfd,(struct sockaddr *) &cliente, sizeof(cliente));
     if(bin == -1){
        perror("\nError al binnear\n");
        return -1;
    }
    strcpy(buff_tx, "HOLA!");

    sendto(socketfd, buff_tx, strlen(buff_tx)+1, 0,(struct sockaddr *) &servidor , sizeof(servidor));
    printf("\nMensaje enviado\n");
    close(socketfd);

    ////////EJERCICIO 7///////////////////////////////////////////
    printf("\nEjecutando Ejercicio 7");
    servidor.sin_port = htons(PORT1);
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
     if(socketfd == -1){
        perror("\nError al crear el socket\n");
        return -1;
    }

    bin = bind(socketfd,(struct sockaddr *) &servidor, sizeof(servidor));
    if(bin == -1){
        perror("\nError al binnear\n");
        return -1;
    }
    
    printf("Envio de mensaje al cliente");
    tam = sizeof(cliente);
    n = recvfrom(socketfd, buff_rx, BUF_SIZE, 0, (struct sockaddr *) &cliente, &tam);
    printf("\nEl mensaje recibido es: %s", buff_rx);
    sendto(socketfd,buff_rx, strlen(buff_rx),0,(struct sockaddr*) &cliente ,tam);
    
    close(socketfd);
    return 0;
}
