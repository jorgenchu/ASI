#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define TAM 256
int fdl,fde, n, pidMonitor;
char buf[TAM];
char buf1[TAM],

#define CLAVE 0x72357116L

void ejercicio1(void)
{
/////LECTURA///
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY);  
    n = read(fdl, buf, sizeof(buf));
    printf("\n mensaje recibido: %s\n", buf);

    //close(fdl);
}

void ejercicio2(void)
{
    printf("\nIntroduce un mensaje por pantalla:\n");
    fflush(stdin);    
    fgets(buf, TAM, stdin);
    __fpurge(stdout);
/////ZONA DE ESCRITURA///
    mkfifo("/tmp/fifo_monitor_2", 0666);
    fde = open("/tmp/fifo_monitor_2", O_WRONLY);
    write(fde, buf, sizeof(buf));
    close(fde);
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY); 
    n = read(fdl, buf1, sizeof(buf1));
    printf("\n Secreto 2: %s\n", buf1);
    printf("\nIntroduce un mensaje por pantalla:\n");
    fflush(stdin);    
    fgets(buf1, TAM, stdin);
    __fpurge(stdout);
    write(fde, buf, sizeof(buf1));
    
    close(fdl);
}

void ejercicio3(void){

typedef struct mensaje{
    long canal;
    char cad[200]; 
}MENSAJE;

int idCola;

MENSAJE m;

idCola = msgget(CLAVE,0666);
msgrcv(idCola,&m,sizeof(m)-sizeof(long),0,0);
printf("Secret: %s", m.cad);




}
int main(int argc, char const *argv[])
{
 

if(argc < 2)
{
    perror("\nFalta parámetro 1.\n");//si es menor que dos es que no le hemos pasado 
    //el parametro
    exit(-1);
}
else
{
   ejercicio1();
   getchar();
   ejercicio2();
   getchar();
   ejercicio3();
   getchar();
   //ejercicio4();

}    
    return 0;
}
