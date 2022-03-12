#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAM 200

int main(int argc, char const *argv[])
{
    int fde, fdl, n;
    char buf[TAM];
    char traducido[1024];
    printf("\nIntroduce un mensaje por pantalla:\n");
    fflush(stdin);    
    fgets(buf, TAM, stdin);
    __fpurge(stdout);
/////ZONA DE ESCRITURA
    mkfifo("/tmp/fifo", 0666);
    fde = open("/tmp/fifo", O_WRONLY);
    write(fde, buf, sizeof(buf));
////ZONA DE LECTURA
    fdl = open ("/tmp/fifo2", O_RDONLY);
    n = read(fdl, traducido, sizeof(traducido));
    printf("\nMensaje traducido: %s", traducido);   


    close(fdl);
    
    return 0;
}
