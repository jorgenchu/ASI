#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define FIFO_RD "/tmp/fifo_rd"
#define FIFO_WR "/tmp/fifo_wr"

#define PORT 10000

#define KEY 0x72357116L

#define SIZE 1024

#define OFF_AC_COUNTER 10
#define N_COUNTERS 4


char *tbl_cmd[]={"HE","AC","OK","SC","SQ","NK",0};
char *tbl_dev_name[]={ "FIFO","TCP","UDP" };
////////////////////////////////////////////////////////

int pidMonitor, hijo1, hijo2, hijo3;
int fifor, fifow;





int main(int argc,char *argv[])
{
char buf[256];


	if(argc != 2) {
		fprintf(stderr,"Missing parameters.\nFormat: %s <pidMonitor>\n",argv[0]);
		exit(20);
	}
    pidMonitor = atoi(argv[1]);
    printf("Press Enter to start exercise:\n");
	fgets(buf,sizeof(buf)-1,stdin);


    hijo1 = fork();
    switch(hijo1)
    {
        case -1: printf("\nError al crear el hijo 1\n");break;
        case 0: 
            fifor = open(FIFO_RD, O_RDONLY);
            fifow = open(FIFO_WR, O_WRONLY);        



            break;
        default:
            printf("\nPadre\n"),
            hijo2 = fork();
            switch(hijo2)
            {
                case -1: printf("\nError al crear el hijo 2\n");break;
                case 0:
                while(1)pause();
                break;
                default:
                    printf("\npadre\n");
                    hijo3 = fork();
                    switch(hijo3)
                    {
                        case -1: printf("\nError al crear el hijo 3");break;
                        case 0: 
                        while(1)pause();
                        break;
                        default:
                        printf("\npadre3\n");
                    }
            }
    }



	
	printf("END EXERCISE\n");
}