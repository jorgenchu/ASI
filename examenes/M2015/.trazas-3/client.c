#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>



#define CLAVE 0x72357116L
#define PORT 3010
#define TAM 100

struct sembuf asumas[3] = {{0,1,0},{1,1,0},{2,1,0}};
struct sembuf arestas[3] = {{0,-1,0},{1,-1,0},{2,-1,0}};

struct sockaddr_in servidor, cliente;
int pidMonitor, socketfd, bin, li, tam, new_socketfd, n, idMem, num, desp1, desp, dato, dato1, res, idSem;
char cad[TAM], cad1[TAM], c, *dir, buff[TAM];



void esperar(){
    wait(NULL);
}

int main(int argc, char const *argv[])
{

    pidMonitor = atoi(argv[1]);

    //Definicion del servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORT);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(servidor.sin_zero), 8);
    ///////////////////////////////////////////
    //Creacion del socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketfd ==-1){
        perror("\n[SERVIDOR]: Error al crear el socket\n");
        return -1;
    }

    bin = bind(socketfd, (struct sockaddr*) &servidor, sizeof(servidor));

    if(bin ==-1){
        perror("\n[SERVIDOR]: Error al binnear\n");
        return -1;
    }

    li = listen(socketfd, SOMAXCONN);
    signal(SIGCHLD, esperar);

    idMem = shmget(CLAVE, 1024,0);  
    dir = shmat(idMem, 0, 0);
    idSem = semget(CLAVE, 3, 0666);
    while(1){
        tam = sizeof(cliente);
        new_socketfd = accept(socketfd, (struct sockaddr *) &cliente, &tam);
        semop(idSem, &asumas[0], 1);
        if(fork()==0){
            close(socketfd);
            while(n=read(new_socketfd, cad, TAM)>0){
                if(cad[0]=='h'){
                    printf("\n[SERVIDOR] Mensaje recibido: %s", cad);
                    strcpy(cad1, "OK\n");           
                    write(new_socketfd, cad1, strlen(cad1));
                }
                else if(cad[0]=='<'){
                    sscanf(cad, "<%d>%c", &desp, &c);
                    printf("%s", cad);
                    if(c == '='){
                        sscanf(cad, "<%d>=%d", &desp, &dato);
                        semop(idSem, &arestas[2], 1);
                        (*(int*)(dir + desp)) = dato;
                        semop(idSem, &asumas[2], 1);
                        sprintf(buff, "\n");
                        write(new_socketfd, buff, strlen(buff));
                    }else if(c=='?'){
                        sscanf(cad, "<%d>=?", &desp);
                        semop(idSem, &arestas[2], 1);
                        dato = (*(int*)(dir + desp));
                        semop(idSem, &asumas[2], 1);
                        sprintf(buff,"%d\n", dato);
                        write(new_socketfd, buff, strlen(buff));
                    }else if(c=='+'){
                        sscanf(cad, "<%d>+<%d>", &desp, &desp1);
                        semop(idSem, &arestas[2], 1);
                        dato = (*(int*)(dir + desp));
                        semop(idSem, &asumas[2], 1); 
                        semop(idSem, &arestas[2], 1);
                        dato1 = (*(int*)(dir + desp1));
                        semop(idSem, &asumas[2], 1);                        
                        res = dato + dato1;
                        sprintf(buff,"%d\n", res);
                        write(new_socketfd, buff, strlen(buff));
                    }
                    
                }
            }
            close(new_socketfd);
            semop(idSem, &arestas[0], 1);
            semop(idSem, &asumas[1], 1);


            exit(0);
        }else{
            close(new_socketfd);
        }

    }

    return 0;
}
