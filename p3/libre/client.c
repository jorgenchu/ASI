#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define CLAVE 0x72357116L
#define TAMMEM 1024
#define TAM 200
#define NUMSEMS 5
struct sembuf asumas[NUMSEMS] = {{0,1,0}, {1,1,0}, {2,1,0}, {3,1,0}, {4,1,0} };
struct sembuf arestas[NUMSEMS] = {{0,-1,0}, {1,-1,0}, {2,-1,0}, {3,-1,0}, {4,-1,0} };

/////////////SEMAFOROS DEFINIDOS///////////////////////
/*
0 : Acceso a la zona del contador de mensajes
1 : Acceso a constador de número de caracteres recibidos
2 : Acceso a contador de sesiones activas
3 : Acceso a conrados de longitud media de los mensajes
4 : Nº Maximos de cliente
*/

/////////////SEGMENTOS DE MEMORIA/////////////////////
/*
0 => Número de mensajes traducidos 
1 => Número de carácteres recibidos
2 => Numero de sesiones activas
3 => Longitud media de los mensajes
*/



int idCola, idMem, idSem;
char *dir, resp;
typedef struct mensaje{
    long tipo; //Estructura que se utilizará para los mensajer
    int pid; //Para el pid del proceso que envía el mensaje
    char men[TAM];
}MENSAJE;



int main(int argc, char const *argv[])
{
    MENSAJE m, m2;
    idCola = msgget(CLAVE, 0666); //Accedemos a la cola por la que se van a enviar los mensajes 
    idMem = shmget(CLAVE, TAMMEM, 0666);//Segmento de memoria de las estadísticas (ACCESO)
    idSem = semget(CLAVE,NUMSEMS,0666); // Accedemos a los semaforos(Acceso)
    
    dir = shmat(idMem, 0,0);//para el acceso a la memoria compartida y vinvularlo con el proceso 
    semop(idSem, &arestas[4], 1);//Restamos 1 al semáforo de clientes
    semop(idSem, &arestas[2],1);//Restamos 1 al semáforo de sesiones activas (Acceso)

    *((int*)dir+2*sizeof(int)) += 1; //Para aumentar en uno el número de sesiones activas(monitor)
    
    semop(idSem,&asumas[2],1);

    do{
       printf("\nIntroduce el mensaje a traducir: ");
       fflush(stdout); 
       
       __fpurge(stdin); 
       fgets(m.men,200,stdin); 
       
       m.pid = getpid(); // Asignaremos nuestro pid al campo pid de la estructura mensaje
       m.tipo = 1L; // Asignaremos un 1 al canal (Este sera el numero que usaremos para indicar que queremos enviarlo al servidor)
       
       msgsnd(idCola,&m,sizeof(m)-sizeof(long),0); //enviamos mensaje
       msgrcv(idCola,&m2,sizeof(m2)-sizeof(long),getpid(),0); //recepción de mensaje
       printf("\n1El mensaje traducido es: %s\n\n",m2.men); 
       fflush(stdout); 
       
       printf("¿Quieres traducir mas mensajes? (s/n) "); 
       fflush(stdout); 
       
       __fpurge(stdin); 
       scanf("%c",&resp);
    }while(resp == 's'); 
    
    semop(idSem,&arestas[2],1);  
    *((int*)dir+2*sizeof(int)) -= 1; //Reducimos en uno la cifra de sesiones activas
    semop(idSem,&asumas[2],1); //Volvemos a poner el semaforo del contador de sesiones en verde (Para que el monitor pueda acceder a el)
    
    shmdt(dir);//Nos desconectamos de la zona de memoria compartida
    
    semop(idSem,&asumas[4],1); //Aumentamos el semaforo de nº maximo de clientes 
    
    return 0;
}



