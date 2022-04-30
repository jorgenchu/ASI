
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>//prototipo funciones manejo de sockets
#include <netinet/in.h>
#include <string.h>

#define PORT 9000 //definimos el puerto que vamos a utilizar
#define BUF_SIZE 1024
struct sockaddr_in servidor, cliente;

//variables para envio
int len_rx, len,tx = 0;
char buff_tx[BUF_SIZE] = "Test de envio";
char buff_rx[BUF_SIZE];

int socketfd;
struct sockaddr_in servidor;

int main(int argc, char const *argv[])
{
     socketfd = socket(AF_INET, SOCK_STREAM, 0);//familia de direcciones, TCP(OC) o UDP (NOC) y 0
    //socket(int dominio, int type, int protocolo) ==> con 0 se selecciona el protocolo automático
    /*protocolo: Indica el protocolo que se va a utilizar. El valor 0 indica que seleccione el protocolo más apropiado (TCP para SOCK_STREAM, UDP para SOCK_DGRAM).*/
    if(socketfd == -1)
        perror("\nError al crear el socket");
    else{
          printf("[CLIENTE]: Socket successfully created..\n"); 
    }

////////////////////////////////////////////////////////////
 servidor.sin_family = AF_INET;//Diferencia entre PF y AF?
    servidor.sin_port = htons(PORT);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);//inet_ntoa para convertir dierección en formato numérico
    /*INADDR_ANY. Esto le indica al sistema que el programa recibirá mensajes por cualquier IP válida de la máquina, en caso de disponer de varias.*/

    bzero(&(servidor.sin_zero), 8); //esto se utiliza simplemente de relleno, para completar la 
///////////////////////////////////////////////////////////
    //conectar con el servidor



    if((connect(socketfd, (struct sockaddr*) &servidor , sizeof(servidor)))!= 0){
    perror("\nError al conectar con el socket");
    return -1;
    }
    printf("\ncliente conectado\n");

   /* send test sequences*/
    write(socketfd, buff_tx, sizeof(buff_tx));     
    read(socketfd, buff_rx, sizeof(buff_rx));
    printf("CLIENT:Received: %s \n", buff_rx);
   
       
    /* close the socket */
    close(socketfd);  //Envio de datos
   
    
    return 0;
}
