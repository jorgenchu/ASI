#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>

#define CLAVE 0x72357116L
#define TAMMEM 1024

struct sembuf resta = {0,-1,0}, suma ={0,1,0}, arestas[2] = {{0,-1,0},{1,-1,0}}, asumas[2] = {{0,1,0},{1,1,0}}; // Declaración de las operaciones que vamos a realizar con los semaforos

int idMem, secreto1, offset, secreto2, idSem, secreto8, secreto10; //Declaración de variables
char *dir,*dsp; //Declaración de variables

int main(int argc, char** argv) {

    // Ejercicio 1
    
    idMem = shmget(CLAVE,TAMMEM,0666); // Accedemos al segmento de memoria creado por el monitor que tiene como clave nuestro dni en hex y tamaño 1024
    dir = shmat(idMem,0,0); // Unimos la zona de memoria a nuestro proceso
    
    secreto1 = *((int*)dir); // Leemos un entero de la zona de memoria creada por el monitor
    
    offset = *((int *)(dir + sizeof(int))); // Leemos un entero que corresponde al offset. Se ubica en la direccion de comienzo + un entero (El secreto 1)
    
    sscanf(dir+offset,"<%d>",&secreto2); // Leemos de la zona correspondiente al offset el entero que corresponde al secreto 2. Usaremos sscanf ya que el secreto tiene el formato <Secreto2>
                                         // y solo queremos extraer el entero correspondiente al mismo.
    
    sleep(3); // Esperamos durante 3 segundos
    shmdt(dir); // Nos desatamos del segmento de memoria
    
    printf("Fin del ejercicio 1.\n");
    getchar();
    
    
    // Ejercicio 2
    
    idMem = shmget(CLAVE,TAMMEM,IPC_CREAT|0666); // Creamos la zona de memoria de tamaño 1024 que tiene como clave nuestro dni en hex
    dir = shmat(idMem,0,0); // Unimos la zona de memoria a nuestro proceso
        
    sprintf(dir,"<%d>",secreto2); // Escribimos el secreto 2 en la zona de memoria con el formato correspondiente
        
    sprintf(dir + secreto2 + 16,"<%d>",secreto1); // Escribimos el secreto 1 en la zona de memoria. La posicion en la que tenemos que escribir será:
                                                  // La dirección de comienzo + secreto2 + 16
    
    sleep(2); 
    shmdt(dir); // Nos desatamos del segmento de memoria
    
    printf("Fin del ejercicio 2.\n");
    getchar();
    
    // Ejercicio 3
    
    idSem = semget(CLAVE,1,IPC_CREAT|0666); 
    semctl(idSem,0,SETVAL,116); 
    
    printf("Fin del ejercicio 3.\n");
    getchar();
    
    // Ejercicio 4
    
    idSem = semget(CLAVE,2,IPC_CREAT|0666); // Creamos un array que contiene dos semaforos y que tiene como clave nuestro dni en hex
    semctl(idSem,0,SETVAL,1); // Asignamos el valor 1
    semctl(idSem,1,SETVAL,2); // Asignamos el valor 2 

    printf("Fin del ejercicio 4.\n");
    getchar();
    
    // Ejercicio 5
    
    idMem = shmget(CLAVE,TAMMEM,0666); // Accedemos a la zona de memoria con clave correspondiente a nuestro dni en hex y de tamaño 1024
    dir = shmat(idMem,0,0); // Unimos la zona de memoria a nuestro proceso
    idSem = semget(CLAVE,1,0666); // Accdemos al semaforo con clave correspondiente a nuestro dni en hex
    
    semop(idSem,&resta,1); 
    secreto8 = *((int*)dir);
    *((int*)dir) = -secreto8;
    sleep(3); 
    semop(idSem,&suma,2); 
    printf("Fin del ejercicio 5.\n");
    getchar();
    
    // Ejercicio 6
    
    idMem = shmget(CLAVE,TAMMEM,0666); 
    dir = shmat(idMem,0,0);
    idSem = semget(CLAVE,1,0666);
    
    semop(idSem,arestas,2); 
    secreto10 = *((int*)dir); 
    *((int*)dir) = -secreto10; 
    sleep(3); 
    
    printf("Fin del ejercicio 6.\n");
    getchar();
    
    // Ejercicio 7
    
    idSem = semget(CLAVE,1,IPC_CREAT|0666);
    idMem = shmget(CLAVE,TAMMEM,0666); 
        
    semop(idSem,&resta,1);
    semctl(idSem,0,IPC_RMID,NULL); // Eliminamos el semaforo
    shmctl(idMem,IPC_RMID,NULL); // Eliminamos la zona de memoria
    
    printf("Fin del ejercicio 7.\n");
    getchar();
    
    return (EXIT_SUCCESS);
}

