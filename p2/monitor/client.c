
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define TAM 256
int fdl,fde, n, pidMonitor;
char buf[TAM];
char buf1[TAM];
char buf2[TAM];

#define CLAVE 0x72357116L
typedef struct mensaje{
    long canal;
    char cad[200]; 
}MENSAJE;


void ejercicio1(void)
{
/////LECTURA///
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY);  
    n = read(fdl, buf, sizeof(buf));
    printf("\n mensaje recibido: %s\n", buf);

    close(fdl);
}

void ejercicio2(void)
{
    printf("\nIntroduce el secreto 1:\n");
    fflush(stdin);    
    fgets(buf, TAM, stdin);
    __fpurge(stdout);
/////ZONA DE ESCRITURA///
    mkfifo("/tmp/fifo_monitor_2", 0666);
    fde = open("/tmp/fifo_monitor_2", O_WRONLY);
    write(fde, buf, sizeof(buf));
    fdl = open("/tmp/fifo_monitor_1", O_RDONLY); 
    n = read(fdl, &buf1, sizeof(buf1));
    printf("\n Secreto 2: %s\n", buf1);
    
    
    write(fde, buf1, sizeof(buf1));
    
    close(fdl);
}

void ejercicio3(void){

    int idCola;

    MENSAJE m;

    idCola = msgget(CLAVE,0666);
    msgrcv(idCola,&m,sizeof(m)-sizeof(long),0,0);
    printf("Clave secreta 4: %s", m.cad);
    printf("Tipo de menasaje : <%ld> ", m.canal);
}

void ejercicio4_5(void)
{
    MENSAJE m2;

    ////////EJERCICIO 4//////////
    int idCola = msgget(CLAVE, IPC_CREAT|0666);  
    printf("\nIntroduce secretos(<333><444>): \n");
    fflush(stdin);    
    fgets(m2.cad, TAM, stdin);
    __fpurge(stdout);
    msgsnd(idCola,&m2,sizeof(m2)-sizeof(long),0);

    ////////EJERCICIO 5//////////
    printf("\nPulsa enter para comenzar el programa 5");
    getchar();
    msgrcv(idCola,&m2,sizeof(m2)-sizeof(long),getpid(),0);
    printf("\nSecreto 6: %s", m2.cad);
   

}
void ejercicio6_7()
{   
    /////EJERCICIO 6///////////////
    int idCola = msgget(CLAVE, IPC_CREAT|0666);
    MENSAJE m3;
    printf("\nPulsa enter para comenzar el programa 6");
    getchar();
    m3.canal = pidMonitor; 
    printf("\nIntroduce secreto 6 (<666>):\n");
    fflush(stdin);    
    fgets(m3.cad, TAM, stdin);  
    msgsnd(idCola,&m3,sizeof(m3)-sizeof(long),0);
    ////////EJERCICIO 7//////////
    printf("\nPulsa enter para comenzar el programa 7");
    getchar();
    msgsnd(idCola,&m3,sizeof(m3)-sizeof(long),0);

    msgctl(idCola,IPC_RMID,NULL);
    unlink("/tmp/fifo_monitor_1");
    unlink("/tmp/fifo_monitor_2");
    printf("\nPractica completada con éxito\n");    
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
    pidMonitor = atoi(argv[1]);
   printf("\nPulsa enter para comenzar el programa");
   getchar();
   ejercicio1();
   printf("\nPulsa enter para comenzar el programa 2");
   getchar();
   ejercicio2();
   printf("\nPulsa enter para comenzar el programa 3");
   getchar();
   ejercicio3();
   printf("\nPulsa enter para comenzar el programa 4");
   getchar();
   ejercicio4_5();
   ejercicio6_7();
   printf("\nPulsa enter para finalizar el programa\n");
   getchar();
}    
    return 0;
}
