
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>


#define CLAVE 0x72357116L
#define PORT 3010
#define BUFF_TAM 100

struct sockaddr_in servidor, cliente;


int pidMonitor, socketfd, bin, li, new_socketfd, tam, intentos =3, n, salir = 0, salircmd = 0, idMem, idSem, num;
char buff_tx[BUFF_TAM], buff_rx[BUFF_TAM], user[BUFF_TAM], pass[BUFF_TAM], resp[BUFF_TAM], *dir;

void esperar(){
    wait(NULL);
}



int main(int argc, char const *argv[])
{

    pidMonitor = atoi(argv[1]);
    //Declaración de las variables del servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PORT);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(servidor.sin_zero),8);
/////////////////////////////////////////////////

    socketfd = socket(AF_INET, SOCK_STREAM ,0);
    if (socketfd==-1)
    {
        perror("\n[Servidor]: Error al crear el socket\n");
        return -1;
    }
    bin = bind(socketfd, (struct sockaddr *) &servidor, sizeof(servidor));
    if (bin == -1)
    {
        perror("\n[Servidor]: Error al binnear\n");
        return -1;
    }
    li = listen(socketfd, SOMAXCONN);
    if (li == -1)
    {
        perror("\n[Servidor]: Error al escuchar\n");
        return -1;   
    }
///////////MEMORIA COMPARTIDA
    idMem = shmget(CLAVE, 1024, 0666);
    dir = shmat(idMem, 0,0);
/////////////////////////////

    signal(SIGCHLD, esperar);

    while(1){
        tam = sizeof(cliente);
        new_socketfd = accept(socketfd,(struct sockaddr *)&cliente, &tam);
        (*(int*)(dir)) += 1;
        if (fork()==0)
        {
            close(socketfd);
            
            while(intentos>0 && salir == 0){
                sprintf(buff_tx, "login:");    
                write(new_socketfd, buff_tx, strlen(buff_tx));
                n = read(new_socketfd, user, BUFF_TAM);
                user[n] = '\0';

                sprintf(buff_tx,"passwd:");
                write(new_socketfd, buff_tx, strlen(buff_tx));      
                n = read(new_socketfd, pass, BUFF_TAM);
                pass[n] = '\0';

                if(strcmp(user, "anonymous\n") == 0 && strcmp(pass, "guest\n") == 0){
                    (*(int*)(dir+sizeof(int))) += 1;
                    (*(int*)(dir+3*sizeof(int))) += 1;
                    salir = 1;
                    printf("\nUsuario correcto\n");
                    fflush(stdout);
                    sprintf(buff_tx, "anonymous:$");
                    write(new_socketfd, buff_tx, strlen(buff_tx));

                    while((n = read(new_socketfd, buff_rx, BUFF_TAM))>0 && salircmd==0){
                       
                        buff_rx[n] = '\0';
                        printf("\nComando: %s\n", buff_rx);
                        fflush(stdout);

                        if (strcmp(buff_rx, "whoami\n") == 0)
                        {
                            printf("\nwhoami\n");
                            sprintf(buff_tx,"anonymous\n");
                            write(new_socketfd, buff_tx, strlen(buff_tx));

                        } else  if (strcmp(buff_rx, "pid\n") == 0)
                        {
                            printf("\npid:  %d\n", getpid());
                            sprintf(buff_tx,"%d\n", getpid());
                            write(new_socketfd, buff_tx, strlen(buff_tx));


                        }else if (strcmp(buff_rx, "quit\n") == 0)
                        {
                            printf("\nquit\n");
                            salircmd == 1;
                            close(new_socketfd);

                        } else if (strcmp(buff_rx, "bad command\n") == 0)
                        {
                            
                            sprintf(resp,"Error\n");
                            sleep(1);
                            printf("%s\n",resp);
                            fflush(stdout);
                            write(new_socketfd, resp, strlen(resp));
                        }///////////A PARTIR DE AQUI EJER 3
                        else if (strcmp(buff_rx, "ncnx\n") == 0)
                        {
                            num = (*(int*)(dir));
                            sprintf(resp,"%d\n", num);
                            write(new_socketfd, resp, strlen(resp));
                            printf("\nNumero conx recibidas: %s\n",resp);
                            fflush(stdout);
                        }else if (strcmp(buff_rx, "nsess\n") == 0)
                        {
                            num = (*(int*)(dir+1*sizeof(int)));
                            sprintf(resp,"%d\n", num);
                            write(new_socketfd, resp, strlen(resp));
                            printf("\nNumero sesiones establecidas: %s\n",resp);
                            fflush(stdout);
                        }else if (strcmp(buff_rx, "nok\n") == 0)
                        {
                            num = (*(int*)(dir+2*sizeof(int)));
                            sprintf(resp,"%d\n", num);
                            write(new_socketfd, resp, strlen(resp));
                            printf("\nNumero csesiones malq: %s\n",resp);
                            fflush(stdout);
                        }else if (strcmp(buff_rx, "active session\n") == 0)
                        {
                            num = (*(int*)(dir+3*sizeof(int)));
                            sprintf(resp,"%d\n", num);
                            write(new_socketfd, resp, strlen(resp));
                            printf("\nNumero sesiones activas: %s\n",resp);
                            fflush(stdout);
                        }

                        if(salircmd != 1){
                            sprintf(buff_tx,"anonymous:$");
                            write(new_socketfd,buff_tx,strlen(buff_tx)); 
                        }

                    }

                    (*(int*)(dir+3*sizeof(int)))-=1;
                }
                if(salir == 0){
                                              
                    *((int*)(dir+2*sizeof(int))) += 1;
                                     
                }
                intentos--;
           
                
            }
            close(new_socketfd);
            exit(0);
        }
        else
        {
            close(new_socketfd);
        }
    }


    
    
    



    return 0;
}
