#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

int hijo1, hijo2;

int main(int argc, char const *argv[])
{
    hijo1 = fork();

    switch(hijo1)
    {
        case -1:
            perror("Error al crear el hijo 1");break;
        case 0:
            //codigo del hijo
            break;
        default: hijo2 = fork();

        switch (hijo2)
        {
            case -1:
                perror("Error al crear el hijo 2");break;
             
            case 0:
            //codigo del hijo
            break;        
        default:
            break;
        }
    }
    return 0;
}
