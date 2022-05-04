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

///////FUNCION PARA NO DEJAR PROCESOS ZOMBIES///////
void esperar(){
    wait(NULL);
}



int main(int argc, char const *argv[])
{
/////////////EJERCICIO 1/////////////////
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
con = connect(socketfd, (struct sockaddr *) &servidor, sizeof(servidor));
if(con ==-1){
    perror("\n[CLIENTE]: Error al conectar\n");
    return -1;
}
printf("\n[CLIENTE]: Conexión realizada con éxito\n");

n = read(socketfd, buff_rx, sizeof(buff_rx));

sec2 = *((int*)buff_rx);
printf("\nsecreto 2: %d\n", sec2);

//pasamos al formato que se pide
sprintf(buff_tx, "<%d>", sec2);
write(socketfd, buff_tx, strlen(buff_tx));
close(socketfd);

/////////////EJERCICIO 2/////////////////
printf("\nEmpezarndo ejecución del ejercicio 2 (SERVIDOR RECURRENTE)\n");
//////////declaración del servidor//////
servidor.sin_family = AF_INET;
servidor.sin_port = htons (PORT1);
servidor.sin_addr.s_addr = htonl(INADDR_ANY);
bzero(&(servidor.sin_zero),8);



/////////////////////////////////////////

socketfd = socket(AF_INET, SOCK_STREAM, 0);
if(socketfd ==-1){
    perror("\n[Servido]: Error al crear el socket\n");
    return -1;
}

bin = bind(socketfd,(struct sockaddr *) &servidor, sizeof(servidor));
if(bin ==-1){
    perror("\n[Servidor]: Error al bienear\n");
    return -1;
}


li = listen(socketfd, SOMAXCONN);
if(li ==-1){
    perror("\n[Servidor]: Error al escuchar\n");
    return -1;
}
        //Señal sighold, que enviaran los hijos cuando han terminado
printf("\nEscuchando\n");
//signal(SIGCHLD, esperar);
 


    while(1){
        tam = sizeof(cliente);//cliente creado en el ejercicio 1

        new_socket = accept(socketfd, (struct sockaddr *) &cliente, &tam);
        if(new_socket ==-1){
            perror("\n[Servidor]: Error al conectar\n");
            return -1;
        }
        printf("\nÉxito al conectar");
        if(fork() == 0){
            close(socketfd);// se cierra el soxket de escucha, este proceso se hereda del proceso padre
        /*
        Creacion de un proceso secundario que recibira la información del cliente y la enviará de vuelta al cliente
        */
            while(read(new_socket, &num, sizeof(int))==sizeof(int))
            {
                
                printf("\nsecreto : %d\n", num);
                //pasamos al formato que se pide y enviamos
                sprintf(buff_tx, "<%d>", num);
                write(new_socket, buff_tx, strlen(buff_tx));

            }
            close(new_socket);// se cierra el soxket de escucha, este proceso se hereda del proceso padre
            exit(0);

        }else{
            close(new_socket);
        }
    
    }     


    return 0;
}

