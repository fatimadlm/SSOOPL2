#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"
#include "parser.h"



int main (int argc, char *argv[])
{
	char buf [BUFSIZ];			/*Variable donde se almacena la orden*/
	while(1)
	{
		imprimir_prompt();		/*Imprime la pantalla incial de la minishell*/ 
		leer_linea_ordenes(buf);	/*Lee el texto introducido desde la minishell y lo almacena en memoria 
						  añadiendo el carácter de fin de linea*/
		if (strcmp(buf,"exit")==0)	/*Comprueba que la cadena introducida es "exit", en caso afirmativo se 
		{				  deja de ejecutar la minishell*/
			break;
		}
		else
	   	{   	
	   		if (ord_interna(buf)!=1)     /*Comprueba la naturaleza de la orden, si la función devuelve un 1
						       es una orden interna*/
	   		{
	   			printf("Ejecutar linea de ordenes ");
	   			ejecutar_linea_ordenes();
	   		}
	   		else
	   		{
	   			printf("Ejecutar orden interna");
	   			ejecutar_ord_interna();
	   		}
	   	}
	}
	return 0;
}
