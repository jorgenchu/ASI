
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
    int fdl, fde, n;
    char buf[1024];
    
/////LECTURA
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY);
    n = read(fdl, buf, sizeof(buf));
    char traducido[n];
    printf("\n mensaje recibido: %s", buf);

  
    close(fdl);
    return 0;
}
