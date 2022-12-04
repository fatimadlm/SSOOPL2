#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

int** crear_pipes(int nordenes)   //Para la creacion de pipes, devuelve una estructura de punteros a arrays de 2 enteros, 
{ 				  // un entero corresponde al descriptor de entrada y el otro al de salida
	int** pipes = NULL;
	int i;
	
	pipes = (int**)malloc(sizeof(int*) * (nordenes - 1)); //Crear espacio para tantas pipes como ordenes -1
	for( i = 0; i < (nordenes - 1 ); i++)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);    //Crear espacio para 2 enteros en cada array
		pipe(pipes[i]); 
	}
	return (pipes);
}

pid_t ejecutar_orden(const char *orden, int entrada, int salida, int *pbackgr)
{
   char **args;
   pid_t pid = 0;
   int indice_ent = -1, indice_sal = -1; //Por defecto, no hay < ni > 

   if ( (args=parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)	//Parser 
   {	
	//En el caso de que no haya argumentos por consola
      	return (-1);
   }
   if(indice_ent != -1)
	{	//En el caso de que haya una redireccion de entrada hacemos una llamada a la funcion que se encarga de la redirección de entrada
	redirec_entrada(args, indice_ent, &entrada);
	}
   if(indice_sal != -1)
	{	//En el caso de que haya una redireccion de salida hacemos una llamada a la funcion que se encarga de la redirección de salida
	redirec_salida(args, indice_sal, &salida);
	}

   pid = fork(); //Creamos el proceso hijo mediante fork()
	
   if(pid == 0)  //Si soy el hijo
   { 
	if(entrada != 0) //Hay redireccionamiento de entrada
	{	
		dup2(entrada,0);
	}
	if(salida != 1) //Hay redireccionamiento de salida
	{	
		dup2(salida,1);
	}
   	if(execvp(args[0], args) < 0) //Ejecucion de la orden correspondiente
	{	
   		printf("Error execvp\n");
		exit(-1);
   	}
   }else{ //Si soy el padre
   	if(entrada!=0)
   	{
   		close(entrada); //Cerramos la entrada si no esta cerrada ya
   	}
   	if(salida!=1)
   	{
   		close(salida); //Cerramos la salida si no esta cerrada ya
   	}
   }
   free_argumentos(args); //Se libera la memoria
   return pid;
}

void ejecutar_linea_ordenes(const char *orden)
{
   int nordenes;
   pid_t *pids=NULL;
   int backgr;
   char *string;
   char* instruccion;
   int **pipes;
   char** ordenes;
   int entrada;
   int salida;


   string = strdup(orden);				    //Duplica el contenido de orden y lo almacena en el puntero "string"
   while((instruccion = strsep(&string, ";")) != NULL){	    //Separa el contenido del puntero "string" de los carácteres ";"
   	ordenes = parser_pipes(instruccion, &nordenes);
   	pipes=crear_pipes(nordenes);                    //Invocar a crear_pipes para crear las necesarias
   	pids = malloc((nordenes)*sizeof(int));		//Reservamos espacio en la memoria para la estructura pid_t
   	for( int i = 0; i < nordenes; i++)		
   	{
   		if( i==0 ) //Si es la orden de la izquierda
   		{
   			if( nordenes > 1) //Si hay más de una orden
   			{
   				salida = pipes[0][1]; 	      
   			}
   			else
   			{
   				salida = STDOUT_FILENO; //Salida estándar  
   			}	
   			pids[0] = ejecutar_orden(ordenes[0], entrada, salida, &backgr);
   		}
   		else if( (i == nordenes - 1) && (nordenes > 1)) //Si es la penúltima orden
   		{
   			entrada = pipes[nordenes-2][0];     	//Entrada para la orden es la orden de la izquierda
   			salida = STDOUT_FILENO;			//Salida estándar     
   			pids[i] = ejecutar_orden(ordenes[i], entrada, salida, &backgr);  //Ejecuta la orden
   		}
   		else //Si es la orden o ordenes del medio
		{
   			entrada = pipes[i-1][0]; //Descriptor de entrada del pipe
   			salida = pipes[i][1];	 //Descriptor de salida del pipe
   			pids[i] = ejecutar_orden(ordenes[i], entrada, salida, &backgr);	 //Ejecuta la orden
   		}
   	}	
		
   	if ((backgr==0) && (pids[nordenes-1]>0))
   	{
   		waitpid(pids[nordenes-1], NULL, 0);	//Espera hasta que se complete la ejecución de la orden
   	}
   }
   	free(pids); //Libera el espacio en la memoria de los procesos y de las pipes
   	free_ordenes_pipes(ordenes,pipes,nordenes);    

}
   
   
   
   
   
   
