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
    char *dominio,*dni,*stringPuerto,cad[TAMCAD],sec14[TAMCAD];

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
    
    // Inicializaremos las variables para hacer de cliente TCP
    cliente.sin_addr.s_addr = htonl(INADDR_ANY);
    cliente.sin_port = htons(puerto);
    cliente.sin_family = AF_INET;
    bzero(&(cliente.sin_zero),8);
    
    servidor.sin_port = htons(puerto);
    servidor.sin_family = AF_INET;
    bzero(&(servidor.sin_zero),8);
    servidor.sin_addr.s_addr = ((struct in_addr*)(h->h_addr))->s_addr;
      
    // Crearemos el socket indicando en el parametro SOCK_STREAM que usaremos el
    // protocolo TCP
    ds = socket(AF_INET,SOCK_STREAM,0);
    
    // Nos conectaremos al socket
    connect(ds,(struct sockaddr*)&servidor,sizeof(servidor));
    
    // user DNI\n
    // Enviaremos la cadena user+dni\n al servidor (el monitor)
    sprintf(cad,"user %s\n",dni);
    write(ds,cad,strlen(cad));
    printf("TX: %s\n",cad);
    
    // ok\n
    // Leeremos la cadena ok\n que nos enviara el servidor (el monitor) en
    // caso de que todo vaya bien
    n = read(ds,cad,strlen(cad));
    cad[n] = '\0';
    printf("RX: %s\n",cad);
    
    // get clave\n
    // Enviaremos la cadena get clave\n al servidor (el monitor)
    sprintf(cad,"get clave\n");
    write(ds,cad,strlen(cad));
    printf("TX: %s\n",cad);
    
    // CLAVE<???>\n
    // Leeremos la cadena CLAVE:+secreto que nos enviará el servidor (el monitor)
    n = read(ds,cad,TAMCAD);
    cad[n] = '\0';
    // Extraeremos de la cadena que hemos leido del servidor el secreto
    // en formato entero
    sscanf(cad,"CLAVE:%s",sec14);
    printf("RX: %s\n",cad);

    
    // Haremos un eco de la clave que hemos recibido del servidor (el monitor)
    write(ds,sec14,strlen(sec14));
    printf("TX: %s\n",cad);

    // bye\n
    // Leeremos el mensaje bye\n del servidor (el monitor)
    n = read(ds,cad,strlen(cad));
    cad[n] = '\0';
    printf("RX: %s\n",cad);

    // Cerraremos el descriptor del servidor
    close(ds);
    
    return (EXIT_SUCCESS);
}

