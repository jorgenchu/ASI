#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define CLAVE 0x72357116L
#define TAMMEM 1024
#define NUMSEMS 5
#define TAMCAD 200

struct sembuf arestas[NUMSEMS] = {{0,-1,0},{1,-1,0},{2,-1,0},{3,-1,0}}; // Array de operacion resta para los semaforos
struct sembuf asumas[NUMSEMS] = {{0,1,0},{1,1,0},{2,1,0},{3,1,0}}; // Array de operacion suma para los semaforo

/*Semaforos
 * 
 * 0 : Acceso a la zona del contador de mensajes traducidos
 * 1 : Acceso a contador de numero de caracteres recibidos                                      
 * 2 : Acceso a contador de sesiones activas                                                     
 * 3 : Acceso a contador de longitud media de los mensajes                                      
 * 4 : Nº max de clientes


 * Segmentos de memoria
 * 
 * 0.Int. Numero de mensajes traducidos     *
 * 1.Int. Numero de caracteres recibidos    *
 * 2.Int. Numero de sesiones activas        *
 * 3.Float. Longitud media de los mensajes  *
 */

int idMem,idSem;
char *dir;

int main(int argc, char** argv) {

    idMem = shmget(CLAVE,TAMMEM,0666); 
    dir = shmat(idMem,0,0); 
    idSem = semget(CLAVE,NUMSEMS,0666); 
    while(1){ 
        semop(idSem,arestas,NUMSEMS-1); 
        
        printf("1.Numero de mensajes traducidos: %d\n",*((int*)dir)); 
        printf("2.Numero de caracteres recibidos: %d\n",*((int*)dir+sizeof(int)));
        printf("3.Numero de sesiones activas: %d\n",*((int*)dir+2*sizeof(int))); 
        printf("4.Longitud media de los mensajes: %.2f\n\n",*((float*)dir+3*sizeof(int))); 
        
        semop(idSem,asumas,NUMSEMS-1); 
        sleep(1);
    }
    
    return (EXIT_SUCCESS);
}

