#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define CLAVE 0x72357116L
#define TAMCAD 20

int pidMonitor, hijo1, hijo2, idCola;

///Estructura de la cola

typedef struct mensaje{
    long canal;
    char cad[TAMCAD];
}MENSAJE;

void ejercicio1(){
    MENSAJE m;
    m.canal = 1L;
    sprintf(m.cad,"H<%d>", getpid());
    msgsnd(idCola, m.cad, sizeof(m)-sizeof(long), 0);


    
}

int main(int argc, char const *argv[])
{
   
    pidMonitor = atoi(argv[1]);
    idCola = msgget(CLAVE, IPC_CREAT|0666);
    sleep(1);
    kill(pidMonitor, SIGUSR1);
    sleep(1);

    ///Recepcion de las sseñales
    signa(SIGUSR1, ejercicio1);
    hijo1 = fork();

    switch (hijo1)
    {
        case -1: printf("\nError al crear el hijo 1");break;

        case 0: 
        //Codigo del hijo 1
            pause();
            while(1){
                pause();
                //m.canal = (long)getpid();
            }

            break;
        default:
            hijo2 = fork();
            switch (hijo2)
            {
            case -1: printf("\nError al crear el hijo 2");break;
            case 0:
            //codigo del hijo 2
                pause();
                while(1){
                    pause();
                    //m.canal = (long)getpid();
                }
                break;

            default:
            //Codigo del padre

                wait(NULL);
                wait(NULL);
            }
            //codigo del padre
    }




    return 0;
}
