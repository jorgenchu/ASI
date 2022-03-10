#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define TAM 200
int hijo1, hijo2;
int tuberia[2];

int main(int argc, char const *argv[])
{
    pipe(tuberia);
    char mensaje[TAM];
    char mensaje1[TAM];
    hijo1 = fork();
    
    switch(hijo1)
    {
        case -1:
            perror("Error al crear el hijo 1");break;
        case 0:
            printf("\nÉxito al crear el hijo 1\n");
            close(tuberia[0]);//cerramos el lado de lectura
            printf("Introduce el mensaje a traducir: ");
            fflush(stdout);
            __fpurge(stdin);
            fgets(mensaje,200,stdin);
            write(tuberia[1], mensaje, strlen(mensaje));            
            break;
        default: hijo2 = fork();

        switch (hijo2)
        {
            case -1:
                perror("Error al crear el hijo 2");break;             
            case 0:
                printf("\nExito al crear el hijo 2\n");
                close(tuberia[1]);
                read(tuberia[0], mensaje1, TAM);
                printf("\n%s\n", mensaje1);

                for(int i = 0; i < TAM; i ++)
                {   
                    mensaje1[i] = toupper(mensaje1[i]);
                }
                printf("\n%s\n", mensaje1);
            break;        
        default:
            wait(NULL);
            wait(NULL);
            break;
        }
    }
    return 0;
}
