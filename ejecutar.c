#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"


pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   //char **aux = 0;
   pid_t pid = 0;

   int indice_ent = -1, indice_sal = -1,entrada = 0, salida = 1; 		/* por defecto, no hay < ni > */
   if ( (args=parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)	//parser 
   {
	//printf ("vacio\n");
      	return -1;
   }
   if(indice_ent != -1){
	redirec_entrada(args, indice_ent, &entrada);
	}
   if(indice_sal != -1){
	redirec_salida(args, indice_sal, &salida);
	}

   pid = fork();

   if(pid == 0)
	{ 
	if(entrada != 0){
		dup2(entrada,0);
	}
	if(salida != 1){
		dup2(salida,1);
	}

   	if(execvp(args[0], args) < 0){
   		printf("Error execvp\n");
		exit(-1);
   		}
   	}
   entrada = 0;
   salida = 1;
   free_argumentos(args);
   return pid;
}

void ejecutar_linea_ordenes(const char *orden)
{
	
   pid_t pid;
   int backgr;
   char *string;
   char *instruccion;


   string = strdup(orden);				       //Duplica el contenido de orden y lo almacena en el puntero "string"
   while((instruccion = strsep(&string, ";")) != NULL){	       //Separa el contenido del puntero "string" de los carácteres ";" y los ejecuta por separado
   		pid = ejecutar_orden(instruccion, &backgr);
   		waitpid(pid, NULL, 0);			       //Espera hasta que se complete la ejecución de la orden	
   }               


}   
