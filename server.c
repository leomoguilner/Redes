#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERROR(x) do { perror(x); exit(NOK); } while(0)

// Constantes
#define OK    		 0
#define NOK          1        

#define NAME         1024
#define BUFSIZE      1024
#define MAXPENDING   1
#define MAXSIZE      1024
#define FECHA        9
#define NOMBRE       255
#define HORA         6
#define MAXIMOTURNOS 100
#define MONTH        12

// Estructura para los registros de turnos
typedef struct registro_turnos turnos;
struct registro_turnos
{
	char fecha[FECHA]    ;
	char nombre[NOMBRE]  ;
	char apellido[NOMBRE];
	char hora[HORA]      ;
	char medico[NOMBRE]  ;
    int  disponible      ;
};

turnos arrayTurnos[MAXIMOTURNOS];
int posicion=0;

// Funcion que inicializa turnos
// incializa calendario de turnos desde archivo, por ahora le puse pocos registros esto es algo sencillo que despues alguno
// puede generar
// la estructura del archivo (fecha,hora, medico,disponiblidad,Nombre paciente,Apellido Paciente) 
// ojo si es 1 disponible para asignar turnos poner en los valores vacios del archivo la palabra NULL
int inicializarTurnos()
{
	// Inicializacion
	int  k=0;
	char fecha[FECHA]    ="0";
	char medico[NOMBRE]  ="0";
	char nombre[NOMBRE]  ="0";
	char apellido[NOMBRE]="0";
	char hora[HORA]      ="0";
	int  disponible      = 0 ;
	FILE *calendario;
	
	// Abre el archivo que contiene los turnos cargados previamente
    calendario = fopen("calendario.dat", "r");
	
	// Si obtuvo el file con los turnos correctamente
    if (calendario!=NULL)
	{

		int cont=0;
		
        while(!feof(calendario))
		{	
            //printf("Entre cantidad %d\n",k );
			fscanf(calendario,"%s %s %s %d %s %s \n",fecha,hora,medico,&disponible,nombre,apellido);	
			//printf("Despues");
			strcpy(arrayTurnos[k].fecha,fecha)      ;	
			strcpy(arrayTurnos[k].hora,hora)        ;
			strcpy(arrayTurnos[k].medico,medico)    ;
			strcpy(arrayTurnos[k].nombre,nombre)    ;
			strcpy(arrayTurnos[k].apellido,apellido);
			arrayTurnos[k].disponible=disponible    ;
			
            k=k+1;
			posicion=k;
		} // while(!feof(calendario))
		return(OK);
	} // if (calendario!=NULL)
	else
	{
        perror(" Error al abrir archivo ...");
        return(NOK);
    } // if (calendario!=NULL)
}

// Muestra turnos para fines de debugeo
void mostarTurnos()
{
	int i=0;
	
    for (i = 0; i <posicion  ; i++)
	{
		printf("Fecha %s  Hora %s Medico %s Disponiblidad %d\n",arrayTurnos[i].fecha,arrayTurnos[i].hora,arrayTurnos[i].medico,arrayTurnos[i].disponible);
	}
}


// Procesa la informacion enviada por el cliente
int procesaInfo(char * mensaje)
{
	char *token;
	token = NULL;
	printf("%s\n","Entre" );
	token = strtok(mensaje, "|");
	printf(" Token %s\n",token );
	int opcion=atoi(token);
	char fecha[FECHA]="0";
	char medico[NOMBRE]="0";
	char nombre[NOMBRE]="0";
	char apellido[NOMBRE]="0";
	char hora[HORA]="0";
	int disponible=0;
	int count=0;
	int respuesta=0;
	
	switch (opcion)
	{
		case 1:
			printf("opcion 1 Consulta \n");
			
            while (token != NULL)
			{
				switch (count)
				{
					case 0:
						printf("Parametro numero: %d  valor :%s\n", count, token);
					break;
					case 1:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(fecha,token);
					break;
					case 2:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(hora,token);
					break;
					case 3:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(medico,token);
					break;
					default:
				
					break;
				}
				token = strtok(NULL,"|");
				count++;
				
	    	} // Fin while (token != NULL)
	    	
	    	
            // Indica que hay disponibilidad para el turno(falta enviar mensaje a cliente send()...)
			respuesta = consultarDisponibilidad(fecha,hora,medico);
			// consulta para una fecha, hora y medico si existe turno disponible devuelve ok o error al cliente
			break;
		case 2:
			// debe verficar que haya disponilidad sino manda mensaje con error que intente otra fecha
			printf("Opcion 2 Alta de turno\n");
			
            while (token != NULL)
			{
				switch (count)
				{
					case 0:
						printf("Parametro numero: %d  valor :%s\n", count, token);
					break;
					case 1:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(fecha,token);
					break;
					case 2:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(hora,token);
					break;
					case 3:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(medico,token);
					break;
					case 4:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(nombre,token);
					break;
					case 5:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(apellido,token);
					break;                    					
					default:
				
					break;
				}
				token = strtok(NULL,"|");
				count++;
				
	    	} // Fin while (token != NULL)			
			
			// En respuesta verificamos si se dio de alta el turno(falta enviar mensaje a cliente send()...)
			respuesta = altaTurno(fecha,hora,medico,nombre,apellido);
			break;
		case 3:
			// debe verificar si existe sino envia error, si borra OK
			printf("opcion 3 Baja\n");
			
            while (token != NULL)
			{
				switch (count)
				{
					case 0:
						printf("Parametro numero: %d  valor :%s\n", count, token);
					break;
					case 1:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(fecha,token);
					break;
					case 2:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(hora,token);
					break;
					case 3:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(medico,token);
					break;
					case 4:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(nombre,token);
					break;
					case 5:
						printf("Parametro numero: %d  valor :%s\n", count, token);
						strcpy(apellido,token);
					break;                    					
					default:
				
					break;
				}
				token = strtok(NULL,"|");
				count++;
				
	    	} // Fin while (token != NULL)			
			
			// En respuesta verificamos si se dio de alta el turno(falta enviar mensaje a cliente send()...)
            respuesta = bajaTurno(fecha,hora,medico,nombre,apellido);
            break;
		default:
			printf("%d\n",opcion );
			break;
	}
	//printf("%s\n","Despues" );
	return respuesta;
} // Fin procesaInfo

void handle_connection(int NS, fd_set * activefds)
{
	char buf[BUFSIZE];			/* Input buffer */
	FILE *fp;
	int n;					/* Stream (converted file des.) */
	int resp=0;
		
	n = recv(NS, buf, BUFSIZE,0);
    //aca esta recibiendo el mensaje con token hacer un case de acuerdo a la opcion que es el primer elemento del mensaje, ver procesainfo
	resp=procesaInfo(buf);            
    buf[n] = '\0';
   
    //printf("Soket FD %d\n",NS );
    strcpy(buf , "");
    strcpy(buf ,"PRUEBAAAAAAAAAAA");
    if (send(NS,buf, strlen(buf),0)== -1) 
    {
         printf("Falla al enviar el mensaje\n");
         //close(client_fd);
    }

    printf("Server:Mensaje enviado: %s\n Cantidad de bytes: %d\n",buf, (int)strlen(buf));
	//FD_CLR(NS, activefds);
}

int consultarDisponibilidad(char fecha[FECHA],char hora[HORA],char medico[NOMBRE])
{	
    int i;
    for (i = 0; i <posicion  ; i++)
	{

        if(strcmp(arrayTurnos[i].fecha,fecha) == 0)
        {
            if(strcmp(arrayTurnos[i].hora,hora) == 0)                                
            { 
                  if(strcmp(arrayTurnos[i].medico,medico) == 0)                                                                 
                  { 
                     if(arrayTurnos[i].disponible == 1)
                     {
                         return 1;
                     }   
                  }   
            }   
        }       
    }    
} // Fin consultarDisponilidad()

int altaTurno(char fecha[FECHA],char hora[HORA],char medico[NOMBRE],char nombre[NOMBRE],char apellido[NOMBRE])
{
    int i;
    int alta=0;
    FILE * fp = fopen("calendario.dat","w+");
    
    if (fp != NULL)
    {
          for (i = 0; i <posicion  ; i++)
	      {

            if(strcmp(arrayTurnos[i].fecha,fecha) == 0)
            {
              if(strcmp(arrayTurnos[i].hora,hora) == 0)                                
              { 
                  if(strcmp(arrayTurnos[i].medico,medico) == 0)                                                                 
                  { 
                     if(arrayTurnos[i].disponible == 1)
                     {
                       strcpy(arrayTurnos[i].nombre,nombre);
                       strcpy(arrayTurnos[i].apellido,apellido);
                       arrayTurnos[i].disponible = 0;
                       
                       alta=1; // Se dio de alta el turno correctamente
                     }   
                  }   
              }   
            }
            fprintf(fp,"%s \t%s \t%s \t%d \t%s \t%s\n",arrayTurnos[i].fecha,arrayTurnos[i].hora,arrayTurnos[i].medico,arrayTurnos[i].disponible,arrayTurnos[i].nombre,arrayTurnos[i].apellido);                     
          }
      fclose(fp);    
    }  // Fin if(fp != NULL)  
    return alta;
} // Fin altaTurno()

int bajaTurno(char fecha[FECHA],char hora[HORA],char medico[NOMBRE],char nombre[NOMBRE],char apellido[NOMBRE])
{
    int baja=0;
    FILE* fp;
    fp = fopen("calendario.dat","w+");
    int i;

    if(fp != NULL)
    {
          for (i = 0; i <posicion  ; i++)
	      {

            if(strcmp(arrayTurnos[i].fecha,fecha) == 0)
            {
              if(strcmp(arrayTurnos[i].hora,hora) == 0)                                
              { 
                  if(strcmp(arrayTurnos[i].medico,medico) == 0)                                                                 
                  {
                       arrayTurnos[i].disponible = 2;
                       
                       baja=1; // Se dio de baja el turno correctamente
                        
                  }   
              }   
            }
            fprintf(fp,"%s \t%s \t%s \t%d \t%s \t%s\n",arrayTurnos[i].fecha,arrayTurnos[i].hora,arrayTurnos[i].medico,arrayTurnos[i].disponible,arrayTurnos[i].nombre,arrayTurnos[i].apellido);                     
          }
      fclose(fp);    
    }  // Fin if(fp != NULL)  
    return baja;
} // Fin bajaTurno()

//--------------------PROGRAMA PRINCIPAL-----------------------
int main(int argc, char *argv[])
{
	fd_set readfds, activefds ;	// the set of read descriptors 
	unsigned short port;	// Port to which server will bind 
	char servhost[NAME];	// Local host name 
	struct sockaddr_in sock;	// INTERNET socket space 
	struct hostent *server;	// Local host information 
	int S;	// fd for socket 
	int NS;	// fd for connected socket 
	int pid;	// used to determine parent or child 
	int i;	// counter to go through FDs in fdset 
 	int optval =1;
	
	if (argc != 2)
	{
		fprintf(stderr,"Modo de uso ./server Port\n");
		exit(NOK);
	}

	port = atoi(argv[1]);

	inicializarTurnos();
	mostarTurnos();

	/*
	 * Get socket - INTERNET DOMAIN - TCP
	 */
	if ((S = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		  fprintf(stderr, "Fallo la creacion del socket!!\n");

	/*
	 * Obtain host name & network address
	 */
 	//ESTO SIRVE PARA QUE BINDEE IGUAL SI ESTA EN USO
	int tr=1;

	// kill "Address already in use" error message
	if (setsockopt(S,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
	    perror("setsockopt");
	    exit(NOK);
	}

	gethostname(servhost, sizeof(servhost));
	
    if ((server = gethostbyname(servhost)) == NULL)
	{
		fprintf(stderr,"%s: unknown host\n",servhost);
		exit(NOK);
	}

	/*
	 * Bind to server address - in network byte order
	 */
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = INADDR_ANY; 
	//memcpy(&sock.sin_addr, server->h_addr, server->h_length);

	/*
	 * Bind socket to port/addr
	 */
	

	if (bind(S, (struct sockaddr *)&sock, sizeof(sock)) < 0)
		ERROR("server: bind");

	/*
	 * Listen on this socket
	 */
	if (listen(S,MAXPENDING) < 0)
		ERROR("server: listen");
	
	/* Initialize the set of active sockets. */
    FD_ZERO (&activefds);
    FD_SET (S, &activefds);
	
	while(NOK)
	{
		/* Block until input arrives on one or more active sockets. */
        readfds = activefds;
    
        if (select (FD_SETSIZE+1, &readfds, NULL, NULL, NULL) < 0)
        {
			perror ("select");
            exit (NOK);
        }
		
		/* Service all the sockets with input pending. */
		for (i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET (i, &readfds))
			{
				if (i == S)
				{
					if ((NS = accept(S,NULL,NULL)) < 0)
						ERROR("server: accept");
					FD_SET(NS, &activefds);	//add the new socket desc to our active connections set
					printf(" NS: %d\n",NS);
				}
				else
				{
					/* Data arriving on an already-connected socket. */
					printf("SOCKET Nro: %d \n",i );
					handle_connection(i, &activefds);
				}
	
			}
		} /* end of for */
	} /* end of while */

	return(0);
}

