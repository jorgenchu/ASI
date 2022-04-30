
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>//prototipo funciones manejo de sockets
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define PORT 9000 //definimos el puerto que vamos a utilizar
#define BUF_SIZE 1024
struct sockaddr_in servidor, cliente;

//variables para envio
int len_rx, len,tx = 0;
char buff_tx[BUF_SIZE] = "Test de envio";
char buff_rx[BUF_SIZE];


int socketfd, ret, addrlen, new_socketfd,n, li;

int main(int argc, char const *argv[])
{

    //INICIALIZACIÓN DEL SERVIDOR
    servidor.sin_family = AF_INET;//Diferencia entre PF y AF?
    servidor.sin_port = htons(PORT);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);//inet_ntoa para convertir dierección en formato numérico
    /*INADDR_ANY. Esto le indica al sistema que el programa recibirá mensajes por cualquier IP válida de la máquina, en caso de disponer de varias.*/

    bzero(&(servidor.sin_zero), 8); //esto se utiliza simplemente de relleno, para completar la 
    //longitud de sockaddr
    
    //CREACION DEL SOCKET

    socketfd = socket(AF_INET, SOCK_STREAM, 0);//familia de direcciones, TCP(OC) o UDP (NOC) y 0
    //socket(int dominio, int type, int protocolo) ==> con 0 se selecciona el protocolo automático
    /*protocolo: Indica el protocolo que se va a utilizar. El valor 0 indica que seleccione el protocolo más apropiado (TCP para SOCK_STREAM, UDP para SOCK_DGRAM).*/
    if(socketfd == -1)
        perror("\nError al crear el socket");
    else{
          printf("[SERVER]: Socket successfully created..\n"); 
    }
    //ASOCIACIÓN DEL SOCKET ==> bind se utiliza para asociar el socket con unos parámetros
    //int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
    ret = bind(socketfd, (struct sockaddr *) & servidor, sizeof(servidor));
    if (ret != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully binded \n");
    }


    ///MODO DE ESCUCHA 
    li = listen(socketfd, SOMAXCONN);//Se habilita el socket para poder recibir conexiones
    //en el backlog se definen el numero maximo de conexiones que puede haber en la cola

    if ( li != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Listening \n"); 
    }


    addrlen = sizeof(cliente);
    while(1){
        
        new_socketfd = accept(socketfd, (struct sockaddr *) &cliente, &addrlen); //aceptas al cliente
        if(new_socketfd< 0){
        perror("Error al conectar con el cliente");
        return -1;
        }else{
        printf("\nCliente conectado");
        while(1){
            len_rx = read(new_socketfd, buff_rx, sizeof(buff_rx));
            if(len_rx == -1){
                perror("\nError al leer los datos");
                return -1;
            }else if(len_rx == 0 ){
                    printf("\nÉxito al conectar con el cliente");
                    close(new_socketfd);
                    break;
            }else{
                write(new_socketfd, buff_tx, strlen(buff_tx));
                printf("\n[SERVER]: %s", buff_rx);
            }

        }

        }
       
    }


    return 0;
}






//falta el tratamiento de errores y luego hacer que el servidor este permanentemente a la escucha
