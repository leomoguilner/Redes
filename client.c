
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5003
#define MAXSIZE 1024
#define FECHA 9
#define NOMBRE 255
#define HORA 6
#define MAX_MENU 3

int menu(void)
{
    int opcion;
    printf("Menu\n\n");
    printf("1.\tConsulta de Disponiblidad\n");
    printf("2.\tAlta\n");
    printf("3.\tBaja\n");
    printf("0.\tSalir\n\n");
    printf("Elija opcion: ");

    while( (scanf(" %d", &opcion) != 1)
          || (opcion < 0)            
          || (opcion > MAX_MENU))    
    {
      fflush(stdin);                 
      printf("No valido\n\n");
      printf("Elija opcion: ");
    }
    return opcion;
}
char * ingreso(int opcion)
{
	char fecha[FECHA];
	char nombre[NOMBRE];
	char apellido[NOMBRE];
	char hora[HORA];
	char mensaje[MAXSIZE-1]; 
    char medico[NOMBRE];
	printf("Ingrese fecha: (ddmmAAAA)\n");
	scanf("%s",fecha);
	switch (opcion)
	{
		case 1:
			printf("Ingrese hora: (hhmm)\n");
			scanf("%s",hora);
            printf("Ingrese Medico:\n");
            scanf("%s",medico);
			sprintf(mensaje,"%d|%s|%s|%s",opcion,fecha,hora,medico);
			break;
		case 2:
			printf("Ingrese hora: (hhmm)\n");
			scanf("%s",hora);
			printf("Ingrese nombre: (hhmm)\n");
			scanf("%s",nombre);
			printf("Ingrese apellido: \n");
			scanf("%s",apellido);
            printf("Ingrese medico: \n");
            scanf("%s",medico);
			sprintf(mensaje,"%d|%s|%s|%s|%s|%s",opcion,fecha,hora,nombre,apellido,medico);
			break;
		case 3:
			printf("Ingrese hora: (hhmm)\n");
			scanf("%s",hora);
			printf("Ingrese nombre: (hhmm)\n");
			scanf("%s",nombre);
			printf("Ingrese apellido: \n");
			scanf("%s",apellido);
			printf("Ingrese medico: \n");
            scanf("%s",medico);
            sprintf(mensaje,"%d|%s|%s|%s|%s|%s",opcion,fecha,hora,nombre,apellido,medico);
		default:
			printf("\nEvento sin definir\n");
		break;
	}
	return  mensaje;

}
int main(int argc, char *argv[])
{
 // iniciamos las estructura del socket y la estructura que tiene los datos del host
    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd,num;
    char buffer[1024];

    char buff[1024];
  //validamos la entrada que tenga los tres argumentos
    if (argc != 3) {
        fprintf(stderr, " Usar el nombre del host y puerto\n");
        exit(1);
        // cuando se produce un error siempre devolvemos 1
    }
   // tomo los datos del host, gethostbyname toma nombre o IP y guarda los datos en he
    if ((he = gethostbyname(argv[1]))==NULL) {
        fprintf(stderr, "No se obtuvioern los datos del servidor \n");
        exit(1);
    }
   //crea el socket especificando el metodo, el moto y el protocolo a usar 0=TCP
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Fallo la creacion del socket\n");
        exit(1);
      // si no puede crear el socket devuelve -1 y sale con el exit -1
    }

   //setea el espacio en memoria - sizeof obtengo del tipo server_info
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET; // datos de la familia de la direc
    server_info.sin_port = htons(PORT); // port TCP
    server_info.sin_addr = *((struct in_addr *)he->h_addr);//direccion de internet
    // establecemos la conexion con el socket y se define el puntero del struct sockaddr
    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0) {
          fprintf(stderr, "Error de conexion\n");// validamos el error de conexion
        exit(1);
    }

    // while para dejer el cleinte abierto hasta que el usuario decida no usarlo mas y puede enviar mas de un mensaje
       while(1) {
       	int opcion=menu();
        if (opcion != 0)
        {
        	strcpy(buffer,ingreso(opcion));
        	printf("%s\n",buffer );
        }
        //printf("Cliente: Ingrese informacion a enviar:\n");
        //fgets(buffer,MAXSIZE-1,stdin);
        if ((send(socket_fd,buffer, strlen(buffer),0))== -1) {
                fprintf(stderr, "Error enviando mensaje\n");
                close(socket_fd);
                exit(1);
        }
        else {
        	// envia el mensaje al server
                printf("Cliente:Mensaje enviado: %s\n",buffer);
                //recibe el mensaje del server
                num = recv(socket_fd,buffer, sizeof(buffer),0);
                // si recibe <=1 cierra la conexion
                if ( num <= 1 )
                {
                        printf("Conexion Cerrada\n");
                        //Sale del while
                        break;
                }

                buff[num] = '\0';
                printf("Client:Mensaje recibido del servidor -  %s\n",buffer);
           }
    }
    //cierra el socket
    close(socket_fd);
}
