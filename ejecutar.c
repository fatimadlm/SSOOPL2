#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"


pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   //char **aux = 0;
   pid_t pid = 0;
   int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
   if ( (args=parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
   {
	//printf ("vacio\n");
      	return -1;
   }
   pid = fork();
   if( pid == 0){ 
   	if(execvp(args[0], args) <0){
   	printf("Error execvp\n");
	exit(-1);
   	}
   }
   free_argumentos(args);
   return pid;
   /*Si la linea de ordenes posee tuberias o redirecciones, podra incluir 
    aqui, en otras fases de la practica, el codigo para su tratamiento.  */
	
	
}

void ejecutar_linea_ordenes(const char *orden)
{
	
   pid_t pid;
   int backgr;
   char *string;
   char *instruccion;


   string = strdup(orden);
   while((instruccion = strsep(&string, ";")) != NULL){
   		pid = ejecutar_orden(instruccion, &backgr);
   		waitpid(pid, NULL, 0);
   }               


}   

