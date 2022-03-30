#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define CLAVE 0x72357116L

typedef struct mensaje{
    long canal;
    char cad[200]; 
}MENSAJE;

int idCola;
char resp;
MENSAJE m;

int main(int argc, char** argv) {

    idCola = msgget(CLAVE,0666);
    
    do{
       printf("Introduce el mensaje a traducir: ");
       fflush(stdout);
       __fpurge(stdin);
       fgets(m.cad,200,stdin);
       m.canal = 1L;
       msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
       
       msgrcv(idCola,&m,sizeof(m)-sizeof(long),0,0);
       printf("El mensaje traducido es: %s\n",m.cad);
       fflush(stdout);
       __fpurge(stdin);
       printf("¿Quieres traducir mas mensajes? (s/n)\n");
       fflush(stdout);
       __fpurge(stdin);
       scanf("%c",&resp);
    }while(resp == 's');
    
    return (EXIT_SUCCESS);
}

