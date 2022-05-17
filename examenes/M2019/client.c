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


#define FIFO_RD "/tmp/fifo_rd"
#define FIFO_WR "/tmp/fifo_wr"

#define PORT 10000

#define CLAVE 0x72357116L

#define SIZE 1024

#define OFF_AC_COUNTER 10
#define N_COUNTERS 4
#define TAMCAD 50


char *tbl_cmd[]={"HE","AC","OK","SC","SQ","NK",0};
char *tbl_dev_name[]={ "FIFO","TCP","UDP" };
char *dir, cad[TAMCAD];
struct sockaddr_in servidor, cliente;

int hijo1, hijo2, hijo3, pidMonitor, fifor, fifow, socketfd, con, tam, bin, idMem, n, cont,num, valor;


int main(int argc,char *argv[])
{
char buf[256];

	if(argc != 2) {
		fprintf(stderr,"Missing parameters.\nFormat: %s <pidMonitor>\n",argv[0]);
		exit(20);
	}

    pidMonitor = atoi(argv[1]);
	printf("Press Enter to start exercise:\n");
	fgets(buf,sizeof(buf)-1,stdin);

    ///////////CREACION DE LA MEMORIA COMPARTIDA///////////
    idMem = shmget(CLAVE,SIZE,0666);
    dir = shmat(idMem,0,0);
    
    for(int i=0;i<7;i++){
        *((int*)(dir+OFF_AC_COUNTER+i*sizeof(int))) = 0;
    }
    //////////////CREACION HIJOS///////////////////////////
    hijo1 = fork();
    switch (hijo1)
    {
    case -1:break;
    case 0:
    ///////////////////////////FIFOS///////////////////////
        fifor = open(FIFO_RD, 0666);
        fifow = open(FIFO_WR, 0666);
        (*(int*)(cad)) = 0;
        (*(int*)(cad + sizeof(int))) = getppid();
        write(fifow, cad, 2*sizeof(int));
        cont = 0;
        while((n = read(fifor, cad, sizeof(cad)))>0){
            cad[n] = '\0';
            if(*cad == 'A'){
                        cont++;
                        *((int*)(dir+OFF_AC_COUNTER)) = cont;
                        valor = *((int*)(cad+2));
                        *((int*)(dir+OFF_AC_COUNTER+4*sizeof(int))) += valor;
                        *((int*)cad) = 1;
                        *((int*)(cad+sizeof(int))) = cont;
                write(fifow, cad, 2*sizeof(int));

            }
            else if(cad[0] == 'S' && cad[1] == 'C'){
                num = (*(int*)(cad +2*sizeof(char)));
                (*(int*)(cad)) = 2;
                (*(int*)(cad + sizeof(int))) = 2;
                (*(int*)(cad + 2*sizeof(int))) = num;
                write(fifow, cad, 3*sizeof(int));
            }else if(*cad == 'S' && *(cad+1) == 'Q'){
                    *((int*)(cad)) = 3;
                    write(fifow,cad,sizeof(int));
             }
                 

        }



    

    break;
    
    default:
        hijo2 = fork();
        switch (hijo2)
        {
        case -1:break;
        case 0:
        ////////////////////////CLIENTE TCP///////////////
        while(1){
            servidor.sin_addr.s_addr= htonl(INADDR_ANY);
            servidor.sin_family = AF_INET;
            servidor.sin_port = htons(PORT);
            bzero(&(servidor.sin_zero), 8);

            cliente.sin_family = AF_INET;
            cliente.sin_addr.s_addr = htonl(INADDR_ANY); 
            cliente.sin_port = htons(PORT);
            bzero(&(cliente.sin_zero), 8);
            
            

            socketfd = socket(AF_INET, SOCK_STREAM, 0);

            con = connect(socketfd, (struct sockaddr *) &servidor, sizeof(servidor));
            if(con ==-1){
                perror("\nError al conectar");
            }
            printf("\n[CIENTE] => CONECTADO");
            (*(int*)(cad)) = 0;
            (*(int*)(cad + sizeof(int))) = getppid();
            write(socketfd, cad, 2*sizeof(int));
                        
            cont = 0;
            while((n = read(socketfd, cad, sizeof(cad)))>0){
                cad[n] = '\0';
                if(cad[0] == 'A'){
                    cont++;
                    (*(int *)(dir + OFF_AC_COUNTER+ sizeof(int))) = cont;
                    valor = (*(int*)(cad + 2));                    
                    (*(int *)(dir + OFF_AC_COUNTER + 5*sizeof(int))) += valor;
                    (*(int*)(cad)) = 1;
                    (*(int*)(cad + sizeof(int))) = cont; 
                    write(socketfd, cad, 2*sizeof(int));

                }
                else if(cad[0] == 'S' && cad[1] == 'C'){
                    num = (*(int*)(cad +2*sizeof(char)));
                    (*(int*)(cad)) = 2;
                    (*(int*)(cad + sizeof(int))) = 5;
                    (*(int*)(cad + 2*sizeof(int))) = num;
                    write(socketfd, cad, 3*sizeof(int));
                }else if(*cad == 'S' && *(cad+1) == 'Q'){
                    *((int*)(cad)) = 3;
                    write(socketfd,cad,sizeof(int));
                }

            }
        }
            

 
            close(socketfd);
        break;
    
        default:

            hijo3 = fork();
            switch (hijo3){
            case -1:break;
            case 0:
            /////////////////////CLIENTE UDP////////////////
            
                servidor.sin_addr.s_addr= htonl(INADDR_ANY);
                servidor.sin_port = htons(PORT);
                servidor.sin_family = AF_INET;
                bzero(&(servidor.sin_zero), 8);

       
                cliente.sin_addr.s_addr = htonl(INADDR_ANY); 
                cliente.sin_port = htons(PORT);
                cliente.sin_family = AF_INET;
                bzero(&(cliente.sin_zero), 8);
            
                
                socketfd = socket(AF_INET, SOCK_DGRAM, 0);
                bin = bind(socketfd, (struct sockaddr*)&cliente, sizeof(cliente));
    
                tam = sizeof(cliente);

                (*(int*)(cad)) = 0;
                (*(int*)(cad + sizeof(int))) = getppid();
                sendto(socketfd, cad, 2*sizeof(int),0, (struct sockaddr *) &servidor, sizeof(servidor));
                cont = 0;
                while((n = recvfrom(socketfd, cad, TAMCAD,0, (struct sockaddr *) &servidor, &tam))>0){
                    cad[n] = '\0';
                if(cad[0] == 'A'){
                    cont++;
                    (*(int *)(dir + OFF_AC_COUNTER + 2*sizeof(int))) = cont;
                    valor = (*(int*)(cad + 2*sizeof(char)));                    
                    (*(int *)(dir + OFF_AC_COUNTER + 6*sizeof(int))) += valor;
                    
                    (*(int*)(cad)) = 1;
                    (*(int*)(cad + sizeof(int))) = cont; 
                    sendto(socketfd, cad, 2*sizeof(int),0, (struct sockaddr *) &servidor, sizeof(servidor));

                }
                else if(cad[0] == 'S' && cad[1] == 'C'){
                    num = (*(int*)(cad +2*sizeof(char)));
                    (*(int*)(cad)) = 2;
                    (*(int*)(cad + sizeof(int))) = 8;
                    (*(int*)(cad + 2*sizeof(int))) = num;
                    sendto(socketfd, cad, 3*sizeof(int),0, (struct sockaddr *) &servidor, sizeof(servidor));
                }else if(*cad == 'S' && *(cad+1) == 'Q'){
                    *((int*)(cad)) = 3;
                    sendto(socketfd, cad, sizeof(int),0, (struct sockaddr *) &servidor, sizeof(servidor));
                    
                }

            
            }

            break;
    
             default:
                printf("Padre:%d\nHijo1:%d\nHijo2:%d\nHijo3:%d\n",getpid(),hijo1,hijo2,hijo3);
                wait(NULL);
                wait(NULL);
                wait(NULL);

        
            }
        
        }
    }












	printf("END EXERCISE\n");
}