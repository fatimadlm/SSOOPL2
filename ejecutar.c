#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

int ** crear_pipes ( int nordenes ) 
{ 
 int ** pipes = NULL ; 
 int i ;  
 pipes = ( int **) malloc ( sizeof ( int *) * ( nordenes - 1)); 
 for ( i = 0; i < ( nordenes - 1); i ++) 
	{ 
	pipes [ i ] = ( int *) malloc ( sizeof ( int ) * 2); 
	if (i=0){
		if (nordenes>1) {
		salida = pipes[0][1];
		}else{
		salida=STDOUT_FILENO;
	pid[0]=ejecutar_orden(ordenes[0],STDIN_FILENO,salida,&backgr);}}
	if ((i=nordenes-1)&&(nordenes>1)){
	entrada=pipes[nordenes-2][0];
	salida= STDOUT_FILENO;
	pid[i]=ejecutar_orden(ordenes[i],entrada,STDOUT_FILENO,&backgr);
	}else{
	//-Falta orden intermedia-
	}
	if(backgr=0&&pid[nordenes-1]>0){
	waitpid();
	free(pid);
	free_ordenes_pipes(ordenes,pipes,nordenes);
	}
return ( pipes );}

pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   //char **aux = 0;
   pid_t pid = 0;
   

   int indice_ent = -1, indice_sal = -1,entrada = 0, salida = 1; 		/* por defecto, no hay < ni > */
   if ( (args=parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)	//parser 
   {	/*en el caso de que no haya argumentos por consola*/
	//printf ("vacio\n");
      	return -1;
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
   }
   entrada = 0;
   salida = 1;
   free_argumentos(args);
   return pid;
}

void ejecutar_linea_ordenes(const char *orden)
{
   int nordenes;
   pid_t pid;
   int backgr;
   char *string;
   char *instruccion;
   int **pipes;
   instruccion=parser_pipes(orden,&nordenes);
   pipes=crear_pipes(nordenes);


   string = strdup(orden);				       //Duplica el contenido de orden y lo almacena en el puntero "string"
   while((instruccion = strsep(&string, ";")) != NULL){	       //Separa el contenido del puntero "string" de los carácteres ";" y los ejecuta por separado
   		pid = ejecutar_orden(instruccion, &backgr);
   		waitpid(pid, NULL, 0);	}		       //Espera hasta que se complete la ejecución de la orden
   if (backgr=0 && (pid[nordenes-1])){
   free_ordenes_pipes(instruccion,pipes,nordenes);
   free(pid);}
   }   
