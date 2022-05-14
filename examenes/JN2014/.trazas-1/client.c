#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>



#define CLAVE 0x72357116L

#define FIFOW "/tmp/72357116J-W"
#define FIFOR "/tmp/72357116J-R"
char cad[20], cad1[20];
int pidMonitor, fifow, fifor, hijo1, hijo2, n, num, idMem, offset, idSem;
char *dir;


///SEMAFOROS//////////////

struct sembuf asumas[4] = {{0,1,0},{1,1,0},{2,1,0},{3,1,0}};
struct sembuf arestas[4] = {{0,-1,0},{1,-1,0},{2,-1,0},{3,-1,0}};

///////////////////////////


void ejercicio1(){
     sprintf(cad, "H<%d>", getpid());
     write(fifow, cad, strlen(cad));
}

void ejercicio2(){
   
    n = read(fifor, cad, 20);
    cad[n] = '\0';
    printf("%s", cad);


    if(cad[0]=='E'){
        ///PARA GUARDAR EL INT EN NUMERO
        num = *((int *)(cad +sizeof(char)));//para que no coja la letra
        sprintf(cad1, "e<%d><%d>", getpid(), num);
        write(fifow, cad1, strlen(cad1));      

    }
    else if(cad[0]== 'R'){
        offset = *((int *)(cad +sizeof(char)));//para que no coja la letra
        num = *((int *)(dir + offset)); 
        sprintf(cad1, "r<%d><%d>", getpid(), num);
        (*(int*)(dir+num)) = getpid();
        write(fifow, cad1, strlen(cad1));   

    }


    

}

int main(int argc, char const *argv[])
{

    pidMonitor =  atoi(argv[1]);
    kill(pidMonitor, SIGUSR1);
    sleep(1);
///////////////EJERCICIO 1////////////////
    
    fifow = open(FIFOW, O_RDWR);

    signal(SIGUSR1, ejercicio1);
    printf("%d", getpid());

///////////////EJERCICIO 2///////////////////
    mkfifo(FIFOR, 0666);
    fifor = open(FIFOR, 0666);

    signal(SIGUSR2, ejercicio2);
///////////////EJERCICIO 3///////////////////

    idMem = shmget(CLAVE, 1024, IPC_CREAT|0666);
    dir = shmat(idMem,0,0);


////////////////EJERCICIO 4///////////////////

//Creacion del semaforo 
idSem = semget(CLAVE,4,IPC_CREAT|0666);
//Inicialización de los semáforos a 0
for(int i = 0; i<4; i++){
    semctl(idSem, i, SETVAL,0);
}














/////////////////////HIJOS///////////////////////
    hijo1 = fork();
    ///////codigo hijo 1
    switch (hijo1)
    {
        case -1: printf("Error al crear el hijo 1"); break;
        case 0:
            pause();
            while (1)
            {
                pause();
                semop(idSem, &arestas[1],1);
                ejercicio2();
            }
        break;
    
    default:
    /////////CODIGO HIJO 2
        hijo2 = fork();
        switch (hijo2)
        {
            case -1: printf("\nError en la creación del hijo 2");break;
            case 0:
            pause();
                while (1)
                {   pause();
                semop(idSem,&arestas[2],1);
                ejercicio2();

                    
                }
            break;

        default:
        //////CODIGO Padre
        pause();
            while(1){
                pause();
               semop(idSem,&arestas[0],1);
               ejercicio2();

            }
            wait(NULL);
            wait(NULL);
         
        }
    }
    
    return 0;
}
