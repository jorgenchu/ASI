#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>



#define CLAVE 0x72357116L
#define TAMCAD 20


int pidMonitor, hijo1, hijo2, idCola, num, idMem, offset, offset1, res1, res2, idSem;
char *dir;
long numop;

struct sembuf asumas= {0,1,0};
struct sembuf arestas= {0,-1,0};
///Estructura de la cola

typedef struct mensaje{
    long canal;
    char cad[TAMCAD];
}MENSAJE;

void ejercicio1(){
    MENSAJE m;
    m.canal = 1L;
    sprintf(m.cad,"H<%d>", getpid());
    msgsnd(idCola, &m, sizeof(m)-sizeof(long), 0);
}

void ejercicio2(){
    MENSAJE m1;
    msgrcv(idCola, &m1, sizeof(m1)-sizeof(long), getpid(),0);
     m1.canal = 1L;

    if(m1.cad[0] == 'E'){
        num = (*(int*)(m1.cad + sizeof(char)));
       
        sprintf(m1.cad,"e<%d><%d>", getpid(),num);
        msgsnd(idCola, &m1, sizeof(m1)-sizeof(long), 0);
    }
    else if(m1.cad[0] == 'R'){
        offset = (*(int*)(m1.cad + sizeof(char)));
        num = (*(int*)(dir + offset));

        sprintf(m1.cad,"r<%d><%d>", getpid(),num);
        (*(int*)(dir + offset))= getpid();
        msgsnd(idCola, &m1, sizeof(m1)-sizeof(long), 0);

    } else if(m1.cad[0] == '+'){
        offset = (*(int*)(m1.cad + sizeof(char)));

        offset1 = (*(int*)(m1.cad + sizeof(char)+sizeof(int)));
        res1 = (*(int*)(dir + offset));
        res2 = (*(int*)(dir + offset1));
        num = res1+res2;
   
        sprintf(m1.cad,"+<%d><%d>", getpid(),num);
        (*(int*)(dir + num))= getpid();
        msgsnd(idCola, &m1, sizeof(m1)-sizeof(long), 0);
    } else if(m1.cad[0] == '*'){
        offset = (*(int*)(m1.cad + sizeof(char)));

        offset1 = (*(int*)(m1.cad + sizeof(char)+sizeof(int)));

        semop(idSem, &arestas, 1);
        res1 = (*(int*)(dir + offset));
        res2 = (*(int*)(dir + offset1));
        semop(idSem, &asumas, 1);
        numop = res1*res2;
   
        sprintf(m1.cad,"*<%d><%ld>", getpid(),numop);
        (*(int*)(dir + numop))= getpid();
        msgsnd(idCola, &m1, sizeof(m1)-sizeof(long), 0);
    }

}

int main(int argc, char const *argv[])
{
   
    pidMonitor = atoi(argv[1]);
    idCola = msgget(CLAVE, IPC_CREAT|0666);
    sleep(1);
    kill(pidMonitor, SIGUSR1);
    sleep(1);

    ////EJERCICIO3
    idMem = shmget(CLAVE, 1024, IPC_CREAT|0666);
    dir = shmat(idMem, 0,0);

    //Creacion semáforos
    idSem = semget(CLAVE, 1, IPC_CREAT|0666);
    //Inicializacion a 0
    semctl(idSem, 0, SETVAL, 0);


    ///Recepcion de las sseñales
    signal(SIGUSR1, ejercicio1);
    hijo1 = fork();

    switch (hijo1)
    {
        case -1: printf("\nError al crear el hijo 1");break;

        case 0: 
        //Codigo del hijo 1

            while(1){
               ejercicio2();
            }

            break;
        default:
            hijo2 = fork();
            switch (hijo2)
            {
            case -1: printf("\nError al crear el hijo 2");break;
            case 0:
            //codigo del hijo 2
               
                while(1){
                    ejercicio2();
                }
                break;

            default:
            //Codigo del padre
                while(1){
                    ejercicio2();   
                }

                wait(NULL);
                wait(NULL);
            }
            //codigo del padre
    }




    return 0;
}
