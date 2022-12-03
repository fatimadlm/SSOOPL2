#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

int** crear_pipes(int nordenes)
{ 
	int** pipes = NULL;
	int i;
	
	pipes = (int**)malloc(sizeof(int*) * (nordenes - 1));
	for( i = 0; i < (nordenes - 1 ); i++)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		pipe(pipes[i]); //creamos tuberia
	}
	return (pipes);
}

pid_t ejecutar_orden(const char *orden, int entrada, int salida, int *pbackgr)
{
   char **args;
   pid_t pid = 0;
   int indice_ent = -1, indice_sal = -1; 		/* por defecto, no hay < ni > */

   if ( (args=parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)	//parser 
   {	/*en el caso de que no haya argumentos por consola*/

      	return (-1);
   }
   if(indice_ent != -1)
	{	/*en el caso de que haya una redireccion de entrada hacemos una llamada a la funcion que se encarga de la instruccion*/
	redirec_entrada(args, indice_ent, &entrada);
	}
   if(indice_sal != -1)
	{	/*en el caso de que haya una redireccion de salida hacemos una llamada a la funcion que se encarga de la isntruccion*/
	redirec_salida(args, indice_sal, &salida);
	}

   pid = fork(); /*creamos el proceso hijo mediante fork()*/
   if(pid == 0)
   { 
	if(entrada != 0)
	{	/*hay redireccionamiento de entrada*/
		dup2(entrada,0);
	}
	if(salida != 1)
	{	/*hay redireccionamiento de salida*/
		dup2(salida,1);
	}
   	if(execvp(args[0], args) < 0)
	{	/*ejecucion de la orden correspondiente*/
   		printf("Error execvp\n");
		exit(-1);
   	}
   }else{
   	if(entrada!=0)
   	{
   		close(entrada); //cerramos la entrada si no esta cerrada ya
   	}
   	if(salida!=1)
   	{
   		close(salida); //cerramos la salida si no esta cerrada ya
   	}
   }

   free_argumentos(args);
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


   string = strdup(orden);				       //Duplica el contenido de orden y lo almacena en el puntero "string"
   while((instruccion = strsep(&string, ";")) != NULL){	       /*Separa el contenido del puntero "string" de los carácteres ";"*/
   	ordenes = parser_pipes(instruccion, &nordenes);
   	pipes=crear_pipes(nordenes);
   	pids = malloc((nordenes)*sizeof(int));		//reservamos espacio en la memoria para la estructura pid_t
   	for( int i = 0; i < nordenes; i++)	
   	{
   		if( i==0 )
   		{
   			if( nordenes > 1) //mas de una orden
   			{
   				salida = pipes[0][1];
   			}
   			else
   			{
   				salida = STDOUT_FILENO;
   			}	
   			pids[0] = ejecutar_orden(ordenes[0], entrada, salida, &backgr);
   		}
   		else if( (i == nordenes - 1) && (nordenes > 1)) //solo una tuberia
   		{
   			entrada = pipes[nordenes-2][0];
   			salida = STDOUT_FILENO;
   			pids[i] = ejecutar_orden(ordenes[i], entrada, salida, &backgr);
   		}
   		else //mas de una tuberia
		{
   			entrada = pipes[i-1][0]; //descriptor de lectura del pipe
   			salida = pipes[i][1];	//descriptor de escritura del pipe
   			pids[i] = ejecutar_orden(ordenes[i], entrada, salida, &backgr);	
   		}
   	}	
		
   	if ((backgr==0) && (pids[nordenes-1]>0))
   	{
   		waitpid(pids[nordenes-1], NULL, 0);	//Espera hasta que se complete la ejecución de la orden
   	}
   }
   	free(pids);
   	free_ordenes_pipes(ordenes,pipes,nordenes);

}
   
   
   
   
   
   
