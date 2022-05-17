#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>




#define CLAVE 0x72357116L
#define OFF_KA_COUNTERS 10 

#define FIFO_RD "/tmp/fifo_rd"
#define FIFO_WR "/tmp/fifo_wr"

#define TAM 50


int hijo1, hijo2, hijo3, pidMonitor, fifor, fifow, socketfd, contador, n, num, numkey, tam, idMem;
char cad[50], *dir;
int puerto;
struct sockaddr_in client, servidor;




int main(int argc, char const *argv[])
{
 char buf[256];
  
	if(argc != 2) {
		fprintf(stderr,"Missing parameters.\nFormat: %s <pidMonitor>\n",argv[0]);
		exit(20);
	}

    pidMonitor = atoi(argv[1]);
	printf("Press Enter to start exercise:\n");
    fgets(buf,sizeof(buf)-1,stdin);

    //////////MEMORIA COMPARTIDA
    idMem = shmget(CLAVE, 1024, 0666);
    dir = shmat(idMem, 0,0);
for(int i=0;i<4;i++){
        *((int*)(dir+OFF_KA_COUNTERS+i*sizeof(int))) = 0;
    }



    ///////////HIJOS//////////////
    hijo1 = fork();
    switch(hijo1)
    {
    case -1:break;
    case 0:
    ///////////////////FIFO///////
        fifor = open(FIFO_RD, 0666);
        fifow = open(FIFO_WR, 0666);
        contador = 0;

        sprintf(cad, "hello %d", getppid());
        write(fifow, cad, strlen(cad));

        while((n = read(fifor, cad, TAM))>0){
            cad[n]='\0';
            if(cad[0] == 'K' && cad[1] == 'A'){
                contador++;
                (*(int*)(dir + OFF_KA_COUNTERS))=contador;
                sprintf(cad, "ka %d", contador);
                write(fifow, cad, strlen(cad));
            }else if(cad[0] == 'S' && cad[1] == 'E'){
                sscanf(cad, "SECRET %d", &num);
                numkey = 2;
                sprintf(cad, "key %d %d", numkey, num);
                write(fifow, cad, strlen(cad));

            }else if(cad[0] == 'Q' && cad[1] == 'U')
            {   
                sprintf(cad, "quit");
                write(fifow, cad, strlen(cad));

            }
        }
        break;
    
    default:
        hijo2 = fork();
        switch(hijo2)
        {
        case -1:break;
        case 0:
        ///////////////////TCP///////
        while(1){
            puerto = 8000 + getppid();
            client.sin_addr.s_addr=htonl(INADDR_ANY);
            client.sin_family= AF_INET;
            client.sin_port=htons(puerto);
            bzero(&(client.sin_zero),8);

            servidor.sin_family= AF_INET;
            servidor.sin_port=htons(puerto);
            bzero(&(servidor.sin_zero),8);

            socketfd = socket(AF_INET, SOCK_STREAM, 0);
            tam = sizeof(servidor);
            connect(socketfd, (struct sockaddr *)&servidor, sizeof(servidor));

            contador = 0;

            sprintf(cad, "hello %d", getppid());
            write(socketfd, cad, strlen(cad));

            while((n = read(socketfd, cad, TAM))>0){
                cad[n]='\0';
                
                if(cad[0] == 'K' && cad[1] == 'A'){
                    contador++;
                    (*(int*)(dir + OFF_KA_COUNTERS + 1*sizeof(int)))= contador;
                    sprintf(cad, "ka %d", contador);
                    write(socketfd, cad, strlen(cad));
                }else if(cad[0] == 'S' && cad[1] == 'E'){
                    sscanf(cad, "SECRET %d", &num);
                    numkey = 5;
                    sprintf(cad, "key %d %d", numkey, num);
                    write(socketfd, cad, strlen(cad));

                }else if(cad[0] == 'Q' && cad[1] == 'U')
                {   
                    sprintf(cad, "quit");
                    write(socketfd, cad, strlen(cad));

                }
            }
            }
            break;
    
            default:
            hijo3 = fork();
            switch(hijo3)
            {
            case -1:break;
            case 0:
            ///////////////////UDP///////
                puerto = 8000 + getppid();
                client.sin_addr.s_addr=htonl(INADDR_ANY);
                client.sin_family= AF_INET;
                client.sin_port=htons(puerto);
                bzero(&(client.sin_zero),8);

                servidor.sin_family= AF_INET;
                servidor.sin_port=htons(puerto);
                bzero(&(servidor.sin_zero),8);

                socketfd = socket(AF_INET, SOCK_DGRAM, 0);
                bind(socketfd, (struct sockaddr *)&client,sizeof(client));

                contador = 0;

                sprintf(cad, "hello %d", getppid());
                sendto(socketfd, cad, strlen(cad),0, (struct sockaddr *) &servidor, sizeof(servidor));

                while((n = recvfrom(socketfd, cad, TAM,0, (struct sockaddr *) &client, &tam))>0){
                    cad[n]='\0';
                    if(cad[0] == 'K' && cad[1] == 'A'){
                        contador++;
                        (*(int*)(dir + OFF_KA_COUNTERS + 2*sizeof(int)))=contador;
                        sprintf(cad, "ka %d", contador);
                        sendto(socketfd, cad, strlen(cad),0, (struct sockaddr *) &servidor, sizeof(servidor));

                    }else if(cad[0] == 'S' && cad[1] == 'E'){
                        sscanf(cad, "SECRET %d", &num);
                        numkey = 8;
                        sprintf(cad, "key %d %d", numkey, num);
                       sendto(socketfd, cad, strlen(cad),0, (struct sockaddr *) &servidor, sizeof(servidor));


                    }else if(cad[0] == 'Q' && cad[1] == 'U'){   
                        sprintf(cad, "quit");
                        sendto(socketfd, cad, strlen(cad),0, (struct sockaddr *) &servidor, sizeof(servidor));

                    }
                }
            break;
    
            default:
                // printf("Padre:%d\nHijo1:%d\nHijo2:%d\nHijo3:%d\n",getpid(),hijo1,hijo2,hijo3);
                kill(pidMonitor,SIGUSR1);
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }      
    
        }   
        
    }

    return 0;
}

