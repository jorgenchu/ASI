#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>


#define TAM 200


int main(int argc, char const *argv[])
{
    int fdl, fde, n;
    char buf[1024];
    
/////LECTURA
    fdl = open("/tmp/fifo", O_RDONLY);
    n = read(fdl, buf, sizeof(buf));
    char traducido[n];
    printf("\n mensaje recibido: %s", buf);

    for(int i = 0; i <n + 1;i++)
    {
       traducido[i] = toupper(buf[i]);
    }
    printf("\nTraducción: %s", traducido);
    close(fdl);

////ENVIO TRADUCIDO
    mkfifo("/tmp/fifo2", 0666);
    fde = open("/tmp/fifo2", O_WRONLY);
    write(fde, traducido, sizeof(traducido));
    close(fde);
    
    return 0;
}
