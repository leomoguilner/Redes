// Librerias
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Constantes
#define ERROR    1
#define PORT     5003
#define MAXSIZE  1024
#define FECHA    9
#define NOMBRE   255
#define HORA     6
#define MAX_MENU 3

// Menu Principal
int menu(void)
{
    int opcion;
    printf("\n\n\t\t\t* * * * * MENU DE OPCIONES * * * * *");
    printf("\n\t\t---------------------------------------------------");
    printf("\n\t\t\t  Solicitud de Turnos ");
    printf("\n\t\t---------------------------------------------------\n\n\n\n");
    printf("\t\t\t\t (1) Consulta de Disponiblidad\n");
    printf("\t\t\t\t (2) Altas de turnos\n");
    printf("\t\t\t\t (3) Bajas de turnos\n");
    printf("\t\t\t\t (0) Salir...");
    printf("\n\n\t\t\t\tElija opcion: ");    

    while( (scanf(" %d", &opcion) != 1)
     || (opcion < 0)            
     || (opcion > MAX_MENU))    
    {
        fflush(stdin);                 
        printf(" No valido\n\n");
        printf(" Elija opcion: ");
    }
 return opcion;
} // Fin int menu(void)

char * ingreso(int opcion)
{
    char fecha[FECHA];
    char nombre[NOMBRE];
    char apellido[NOMBRE];
    char hora[HORA];
    char mensaje[MAXSIZE-1];
    char medico[NOMBRE];
    int  ind; // 1. Fecha; 2. Hora ; 3. Nombre/Apellido
    int  sw;
    
    // Validamos la fecha ingresada.
    while(1)
    {
        
        printf("Ingrese Fecha: (ddmmAAAA)\n ");
        scanf("%s", fecha);
        
        sw = validarDato(1,fecha);
        
        if(sw == 0)
        {
            break;
        }
        else
        {
            printf(" Fecha no valida,ingrese el formato indicado...\n");
            //getchar();
        }
    } // Fin  fecha ingresada.
    
    
    // Validamos la Hora ingresada.
    while(1)
    {
        
        printf("Ingrese hora: (hhmm)\n ");
        scanf("%s", hora);
        
        
        sw = validarDato(2,hora);
        
        if(sw == 0)
        {
            break;
        }
        else
        {
            printf(" Hora no valida,ingrese el formato indicado...\n");
            //getchar();
        }
    }  // Fin Hora ingresada
    
    // Validamos Medico ingresado.
    while(1)
    {
       
        printf("Ingrese nombre medico:\n ");
        scanf("%s",medico);
        
        sw = validarDato(3,medico);
        
        if(sw == 0)
        {
            break;
        }
        else
        {
            printf(" Medico no encontrado \n ");
        }
    } // Fin Medico ingresado
    
    if(opcion == 1)
    {
        sprintf(mensaje,"%d|%s|%s|%s",opcion,fecha,hora,medico);
        return mensaje;
    }
    
    
    // Validamos Nombre ingresado.
    while(1)
    {
        printf("Ingrese nombre:\n ");
        scanf("%s",nombre);
        
        sw = validarDato(3,nombre);
        
        if(sw == 0)
        {
            break;
        }
        else
        {
            printf(" Nombre no valido...\n");
            //getchar();
        }
    } // Fin Nombre ingresado
    
    //Validamos Apellido ingresado.
    while(1)
    {
        
        printf("Ingrese Apellido:\n ");
        scanf("%s",apellido);
        
        sw = validarDato(3,apellido);
        
        if(sw == 0)
        {
            break;
        }
        else
        {
            printf(" Apellido no valido...\n");
            //getchar();
        }
    } // Fin Apellido ingresado
    sprintf(mensaje,"%d|%s|%s|%s|%s|%s",opcion,fecha,hora,nombre,apellido,medico);
    return  mensaje;
} // Fin char * ingreso(int opcion)

// Valida Fecha,Hora,Medico,Nombre,Apellido
int validarDato ( int ind , char dato [ 50 ] ) 
{ 
    int i = 0 , sw = 0 , j ; 

    j = strlen ( dato ) ; 

    switch ( ind ) 
    { 
        case 1 : // Valida Fecha 
            if ( j == 8 ) 
            { 
                while ( i < j && sw == 0 ) 
                {     
                    if ( isdigit ( dato [ i ] ) != 0 ) 
                        i ++; 
                    else 
                        sw = 1 ;                          
                } 
            } 
            else 
                sw = 1 ; 
        break ;    
        case 2 : // Valida Hora               
            if ( j == 4 ) 
            { 
                while ( i < j && sw == 0 ) 
                {     
                    if ( isdigit ( dato [ i ] ) != 0 ) 
                        i ++; 
                    else 
                        sw = 1 ;                          
                } 
            } 
            else 
                sw = 1 ;     
        break ;       
        case 3 : // Valida Nombre/Apellido/Medico 
            while ( i < j && sw == 0 ) 
            { 
                if ( isalpha ( dato [ i ] ) != 0 ) 
                    i ++; 
                else 
                    sw = 1 ;                          
            }       
        default : 
        break ;            
        } // switch(ind) 

    return sw ; 
    } // Fin int validarDato(char indicador, char numero[50])

//--------------------PROGRAMA PRINCIPAL-----------------------
int main(int argc, char *argv[])
{
    // Iniciamos las estructura del socket y la estructura que tiene los datos del host
    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd,num;
    char buffer[1024];
    char buff[1024];
   // Validamos la entrada que tenga los tres argumentos

    if (argc != 2) 
    {
        fprintf(stderr, " Ingresar puerto\n");
        exit(ERROR);
           // cuando se produce un error siempre devolvemos 1
    }

   // Tomo los datos del host, gethostbyname toma nombre o IP y guarda los datos en he
    if ((he = gethostbyname(argv[1]))==NULL) 
    {
         fprintf(stderr, "No se obtuvieron los datos del servidor \n");
         exit(ERROR);
    }

   // Crea el socket especificando el metodo, el protocolo a usar y 0 por default
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) 
    {
         fprintf(stderr, "Fallo la creacion del socket\n");
         exit(ERROR);
         // si no puede crear el socket devuelve -1 y sale con el exit -1
    }
    printf("Socket FD creado %d\n",socket_fd );
   // Setea el espacio en memoria - sizeof obtengo del tipo server_info
    memset(&server_info, 0, sizeof(server_info));

    server_info.sin_family = AF_INET;   // datos de la familia de la direc
    server_info.sin_port = htons(PORT); // port TCP
    server_info.sin_addr.s_addr = INADDR_ANY; //direccion de internet
   
   // establecemos la conexion con el socket y se define el puntero del struct sockaddr
    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0) 
    {
       fprintf(stderr, "Error de conexion\n"); // validamos el error de conexion
       exit(ERROR);
    }

   // while para dejer el cleinte abierto hasta que el usuario decida no usarlo mas y puede enviar mas de un mensaje
    while(1) 
    {
        int opcion=menu();
        if (opcion != 0)
        {

            strcpy(buffer, ingreso(opcion));
            printf("%s\n",buffer );
        }
        else
        {
            printf("%s\n","Cierra Socket\n" );
            close(socket_fd);
            return;
        }

        if ((send(socket_fd,buffer, strlen(buffer),0))== -1) 
        {
            printf("%s\n","Error enviando mensaje\n" );
            close(socket_fd);
            exit(ERROR);
        }
        else 
        {
            printf("Cliente:Mensaje enviado: %s\n",buffer);
            //recibe el mensaje del server
            //printf(" File descriptor socket %d\n", socket_fd);
            buffer[1024] = '\0';
            num = recv(socket_fd,buffer, sizeof(buffer),0);
                         // si recibe <=1 cierra la conexion
            if ( num <= 1 )
            {
                printf("Conexion Cerrada\n");
                break;
            }
             buff[num] = '\0';
             printf("Client:Mensaje recibido del servidor -  %s\n",buffer);
        }
    }
    //cierra el socket
    close(socket_fd);
} // Fin main()