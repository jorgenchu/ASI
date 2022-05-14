#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 3000
#define PORT1 3001
#define BUF_SIZE 100

struct sockaddr_in servidor, cliente;
/////////////DEFINICION DE VARIABLES//////////////////
int socketfd, new_socket = 0;
//////////////Tratamiento de errores y más//////////////////
int bin, con, n, sec2, li, tam, num= 0;
/////////////////////////////////////////////////////
char buff_tx[BUF_SIZE];
char buff_rx[BUF_SIZE];


int main(int argc, char const *argv[])
{
    ////////////EJERCICIO 1/////////////////
socketfd = socket(AF_INET, SOCK_STREAM, 0);
/////////////DEFINICION CLIENTE//////////

cliente.sin_family = AF_INET;
cliente.sin_port = htons(PORT);
cliente.sin_addr.s_addr = htonl(INADDR_ANY);
bzero(&(cliente.sin_zero), 8);
/////////////DEFINICION CLIENTE//////////
servidor.sin_family = AF_INET;
servidor.sin_port = htons (3000);
bzero(&(servidor.sin_zero),8);
/////////////////////////////////////////
    /////////////////EJERCICIO 3/////////////////////////////
printf("\nErr");
getchar();
//Definición de las variables de UDP(Cliente)
cliente.sin_port = htons(PORT);
//servidor.sin_port = htons (PORT);

socketfd = socket(AF_INET, SOCK_DGRAM, 0);

if(socketfd==-1){
    perror("\n[Servidor]: Error al crear el\n");
    return -1;
}
///Publicación de los datos del cliente
bin = bind(socketfd, (struct sockaddr *) &cliente, sizeof(cliente));
if(bin ==-1){
    perror("\n[Servidor]: Error al bienear\n");
    return -1;
}
printf("\nExito al binnear");
//solicitud del secreto
strcpy(buff_tx, "<\?\?\?>");
sendto(socketfd, buff_tx, strlen(buff_tx), 0, (struct sockaddr *) &servidor, sizeof(servidor));


    return 0;
}

