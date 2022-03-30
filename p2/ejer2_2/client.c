#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define CLAVE 0x72357116L
#define TAM 20

typedef struct mensaje{
    long canal;
    int pidCliente;
    char cad[100];
}MENSAJE;

int idCola;
MENSAJE m,m2;

char menu(){
    char opc;
    
    printf("\n1. Enviar mensajes aleatorios.\n");
    printf("2. Enviar mensajes desde teclado.\n");
    printf("3. Fin del programa.\n");
    printf("Su opcion: ");
    
    fflush(stdout);
    __fpurge(stdin);
    scanf("%c",&opc);
    
    return opc;
}

void generadorCadena(){
    char letras[] = "abcdefghijklmnopqrstuvwxyz";
    
    for(int i=0;i<((rand()%20)+1);i++){
        m.cad[i] = letras[rand()%(strlen(letras)-1)];
    }
    m.cad[strlen(m.cad)-1] = '\0';

}


void aleatorio(){
    
    for(int i=0;i<(rand()%10)+1;i++){
        generadorCadena();
        m.canal = 1L;
        m.pidCliente = getpid();
        msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
        msgrcv(idCola,&m,sizeof(m)-sizeof(long),getpid(),0);
        printf("\nMensaje traducido: %s\n",m.cad);
    }
    
}

void teclado(){
    printf("\nIntroduzca la cadena a traducir: ");
    fflush(stdout);
    __fpurge(stdin);
    fgets(m.cad,100,stdin);
    m.cad[strlen(m.cad)-1] = '\0';
    m.canal = 1L;
    m.pidCliente = getpid();
    msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
    msgrcv(idCola,&m2,sizeof(m2)-sizeof(long),getpid(),0);
    printf("\nEl mensaje traducido es: %s\n",m2.cad);
}

int main(int argc, char** argv) {

    char opc;
    srand((unsigned) time(NULL));
    
    idCola = msgget(CLAVE,0666);
    
    do{
        opc = menu();
        
        switch(opc){
            case '1': aleatorio(); break;
            case '2': teclado(); break;
            case '3': printf("\nFin del programa.\n"); break;
            default: printf("\nLa opcion introducida no es correcta.\n");
        }
    }while(opc != '3');
    
    
    return (EXIT_SUCCESS);
}

