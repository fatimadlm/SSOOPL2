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
   	if (pid != 0){
	printf ("vacio");
	}else{
	pid = fork(); //creamos minishell hija
	}
      	return(-1);
   }else if( (pid == 0) && (execvp(args[0], args) == -1)){
   	printf("No encontrada\n");
	return pid;
   }else{
	printf ("ejecutar2");
	free_argumentos(args);
	return pid;
   }
   /*Si la linea de ordenes posee tuberias o redirecciones, podra incluir 
    aqui, en otras fases de la practica, el codigo para su tratamiento.  */
	
	
}

void ejecutar_linea_ordenes(const char *orden)
{
	
   //pid_t pid;
   int backgr;
	

	
   /* Si la orden es compuesta, podra incluir aqui, en otra fase de la 
   practica, el codigo para su tratamiento  */                        
 	char *arg2[3] = {"sleep","10","&"};
	execvp(arg2[0], arg2);

   ejecutar_orden(orden, &backgr);
   


}   

