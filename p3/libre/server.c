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

int idCola, idMem, idSem;
char *dir;
typedef struct mensaje{
    long tipo; //Estructura que se utilizará para los mensajer
    int pid; //Para el pid del proceso que envía el mensaje
    char cad[TAM];
}MENSAJE;

void longMedia(){ // Funcion que calcula la longitud media de los mensajes
    int numCaracteres; // Declaracion de variables
    int numMensajes;
    float longMedia;
    
    semop(idSem,&arestas[1],1); // Esperamos a poder acceder a la zona de memoria que guarda el numero de caracteres recibidos
    numCaracteres = *((int*)dir+sizeof(int)); // Extraemos a una variable el numero de caracteres recibidos
    semop(idSem,&asumas[1],1); // Ponemos el semaforo en verde para que otros procesos puedan acceder al segmento de memoria compartida
    
    semop(idSem,&arestas[0],1); // Esperamos a poder acceder a la zona de memoria que guarda el numero de mensajes traducidos
    numMensajes = *((int*)dir); // Extraemos a una variable el numero de mensajes traducidos
    semop(idSem,&asumas[0],1); // Ponemos el semaforo en verde para que otros procesos puedan acceder al segmento de memoria compartida
    
    semop(idSem,&arestas[3],1); // Esperamos a poder acceder a la zona de memoria que guarda la longitud media de los mensajes recibidos
    
    if(numCaracteres == 0 || numMensajes == 0){ // Comprobamos que ningun termino de la division sea 0
        *((float*)dir+3*sizeof(int)) = 0; // Si alguno de los dos terminos es 0 la longitud media de los mensajes sera 0
    }else{
        *((float*)dir+3*sizeof(int)) = (float) numCaracteres/numMensajes; // Si ninguno de los dos terminos es 0 la longitud media de los mensajes sera el numero de caracteres recibidos entre el numero de mensajes traducidos
    }
    
    semop(idSem,&asumas[3],1); // Ponemos el semaforo en verde para que otros procesos puedan acceder al segmento de memoria compartida
}


int main(int argc, char const *argv[])
{
    MENSAJE m,m2; 
    idCola = msgget(CLAVE, IPC_CREAT|0666); //creación de la cola de mensajes
    idMem = shmget(CLAVE,TAMMEM, IPC_CREAT|0666);//creacion del segmento de memoria
    dir = shmat(idMem, 0, 0); //se vincula el proceso al segmento de memoria
    idSem = semget(CLAVE, NUMSEMS, IPC_CREAT|0666);//Creación de los semáforos

    ////////////////////////////INICIALIZACIÓN DE SEMAFOROS////////////////////
for(int i=0;i<NUMSEMS-1;i++) // Inicializamos los semaforos a 1 para que se pueda acceder a las zonas de memoria compartida
    semctl(idSem,i,SETVAL,1);

semctl(idSem,4,SETVAL,2); // Inicializamos el semaforo de nº maximo de clientes a 2

 for(int i=0;i<3;i++){ // Inicializamos a 0 los contadores de las zonas compartidas
        semop(idSem,&arestas[i],1); // Esperamos a poder acceder a la zona de memoria que guarda nuestras estadisticas
        *((int*)dir+i*sizeof(int)) = 0; 
        semop(idSem,&asumas[i],1); // Ponemos el semaforo en verde para que otros procesos puedan acceder a los segmentos de memoria compartida
    }
  semop(idSem,&arestas[3],1); // Esperamos a poder acceder a la zona de memoria que guarda nuestras estadisticas
    *((float*)dir+3*sizeof(int)) = 0; // Inicializamos a 0 los contadores de las zonas compartidas
    semop(idSem,&asumas[3],1); // Ponemos el semaforo en verde para que otros procesos puedan acceder a los segmentos de memoria compartida
    
    while(1){
        msgrcv(idCola,&m,sizeof(m)-sizeof(long),0,0);
        printf("Mensaje recibido\n"); 
        for(int i=0;i<strlen(m.cad);i++){ // Recorremos el mensaje enviado por el cliente
            if(m.cad[i] != '\0' && m.cad[i] != '\n'){ // Evitamos contar los saltos de linea y los caracteres de fin de cadena
                semop(idSem,&arestas[1],1); // Esperamos a poder acceder a la zona de memoria que guarda el contador de caracteres recibidos
                *((int*)dir+sizeof(int)) += 1; // Aumentamos en 1 el numero de caracteres recibidos
                semop(idSem,&asumas[1],1); // Ponemos el semaforo en verde para que otros procesos puedan acceder al segmento de memoria compartido
            }
            m2.cad[i]=toupper(m.cad[i]); // Traducimos cada letra de la cadena a mayusculas
        }
        
        m2.cad[strlen(m.cad)] = '\0'; // Añadimos el caracter de fin de cadena
        
        semop(idSem,&arestas[0],1); // Esperamos a poder acceder a la zona de memoria que guarda el contador de mensajes traducidos
        *((int*)dir) += 1; // Aumentamos en 1 el numero de mensajes traducidos
        semop(idSem,&asumas[0],1);
        
        m2.tipo = m.pid; 
        m2.pid = getpid(); 
        
        longMedia(); 
        
        msgsnd(idCola,&m2,sizeof(m2)-sizeof(long),0); 
        
        printf("Mensaje traducido.\n\n"); 
    }
    
    return (EXIT_SUCCESS);
}
