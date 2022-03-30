#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>

#define CLAVE 0x72357116L

typedef struct mensaje{
    long canal;
    char cad[200]; 
}MENSAJE;

int idCola;
MENSAJE m;

int main(int argc, char** argv) {

    idCola = msgget(CLAVE,IPC_CREAT|0666);
    
    while(1){
        
        msgrcv(idCola,&m,sizeof(m)-sizeof(long),1,0);
        for(int i=0;i<strlen(m.cad);i++){
            m.cad[i] = toupper(m.cad[i]);
        }
    
       
        msgsnd(idCola,&m,sizeof(m)-sizeof(long),0);
    }
    
    return (EXIT_SUCCESS);
}
