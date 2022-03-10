

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define TAM 200




int main(int argc, char const *argv[])
{

    
    char mensaje [TAM];
    printf("\nIntroduce un mensaje por pantalla:\n");
    fflush(stdin);
    __fpurge(stdout);
    fgets(mensaje, 200, TAM);
    


    return 0;
}