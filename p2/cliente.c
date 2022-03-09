#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define TAM 200
int hijo1, hijo2;

int main(int argc, char const *argv[])
{
    char mensaje[TAM];
    hijo1 = fork();
    
    switch(hijo1)
    {
        case -1:
            perror("Error al crear el hijo 1");break;
        case 0:
            printf("\nÉxito al crear el hijo 1\n");
            printf("Introduce el mensaje a traducir: ");
            fflush(stdout);
            __fpurge(stdin);
            fgets(mensaje,200,stdin);
            break;
        default: hijo2 = fork();

        switch (hijo2)
        {
            case -1:
                perror("Error al crear el hijo 2");break;             
            case 0:
                printf("\nExito al crear el hijo 2\n");
            break;        
        default:
            wait(NULL);
            wait(NULL);
            break;
        }
    }
    return 0;
}
