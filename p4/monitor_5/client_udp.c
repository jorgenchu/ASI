#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#define TAMCAD 50

int main(int argc, char** argv) {

    struct sockaddr_in cliente,servidor;
    struct hostent *h;

    int ds,dc,tam,n,puerto;
    char *dominio,*dni,*stringPuerto,cad[TAMCAD],sec17[TAMCAD];

    // Usaremos la funcion strtok para separar en cadenas la informacion que
    // recibimos por parametros
    // En el primer caso obtendremos la cadena que hay hasta el simbolo @
    // que corresponderá con el dni
    dni = strtok(argv[1],"@");
    // Obtendremos la cadena que hay desde el puntero de la funcion strtok (que
    // será despues de la @, por eso pondremos null) que corresponderá al dominio
    dominio = strtok(NULL,":");
    // Obtendremos la cadena que hay desde el puntero de la funcion strtok (que
    // será despues de la :, por eso pondremos null) que corresponderá con el puerto
    stringPuerto = strtok(NULL,"");
    // Convertiremos el string que contiene el puerto en un entero
    puerto = atoi(stringPuerto);
    
    // Usaremos la funcion gethostbyname que devolverá una estructura de tipo
    // hostent para el dominio correspondiente
    h= gethostbyname(dominio);
    
    // Inicializaremos las variables para hacer de cliente UDP
    cliente.sin_addr.s_addr = htonl(INADDR_ANY);
    cliente.sin_port = htons(puerto);
    cliente.sin_family = AF_INET;
    bzero(&(cliente.sin_zero),8);
    
    servidor.sin_port = htons(puerto);
    servidor.sin_family = AF_INET;
    bzero(&(servidor.sin_zero),8);
    servidor.sin_addr.s_addr = ((struct in_addr*)(h->h_addr))->s_addr;
      
    // Crearemos el socket indicando en el parametro SOCK_DGRAM que usaremos el
    // protocolo UDP
    ds = socket(AF_INET,SOCK_DGRAM,0);
    
    tam = sizeof(cliente);
    
    // Publicaremos los datos del cliente
    bind(ds,(struct sockaddr*)&cliente,tam);
    
    // GET CLAVE DNI
    // Enviaremos la cadena GET CLAVE+dni al servidor (el monitor) 
    sprintf(cad,"GET CLAVE %s",dni);
    sendto(ds,cad,strlen(cad),0,(struct sockaddr*)&servidor,sizeof(servidor));
    printf("TX: %s\n",cad);
    
    // CLAVE<999>\n
    // Leeremos del servidor la clave 17
    n = recvfrom(ds,cad,strlen(cad),0,(struct sockaddr*)&cliente,&tam);
    cad[n] = '\0';
    // Extraeremos el entero correspondiente al secreto 17 de la cadena que
    // hemos recibido del servidor (el monitor)
    sscanf(cad,"CLAVE:%s",sec17);
    printf("RX: %s\n",cad);
    
    // Haremos un eco de la clave que nos ha enviado el servidor (el monitor)
    sprintf(cad,"%s",sec17);
    sendto(ds,cad,strlen(cad),0,(struct sockaddr*)&servidor,sizeof(servidor));
    printf("TX: %s\n",cad);

    // Cerraremos el descriptor del servidor
    close(ds);
    
    return (EXIT_SUCCESS);
}
