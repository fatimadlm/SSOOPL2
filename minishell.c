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

static void manejar_sigchild(int signo)
{
	int estado;
	waitpid(-1, &estado, WNOHANG); /* ¡EXPLICAR Y COMPRENDER BIEN! */
}

int main (int argc, char *argv[])
{
	char buf [BUFSIZ];			/*Variable donde se almacena la orden*/
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = manejar_sigchild;
	sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;
	sigaction(SIGCHLD, &sa, NULL);
	
	while(1)
	{
		imprimir_prompt();		/*Imprime la pantalla incial de la minishell*/ 
		leer_linea_ordenes(buf);	/*Lee el texto introducido desde la minishell y lo almacena en memoria 
						  añadiendo el carácter de fin de linea*/
		if (strcmp(buf,"exit")==0)	/*Comprueba que la cadena introducida es "exit", en caso afirmativo se deja de ejecutar la minishell*/
		{				 
			break;
		}
		else
	   	{   	
	   		if (es_ord_interna(buf)!=1)     /*Comprueba la naturaleza de la orden, si la función devuelve un 1
						       es una orden interna*/
	   		{
	   			printf("Ejecutar linea de ordenes ");
	   			ejecutar_linea_ordenes(buf);
	   		}
	   		else
	   		{
	   			printf("Ejecutar orden interna");
	   			ejecutar_ord_interna(buf);
	   		}
	   	}
	}
	//exit(EXIT_SUCCESS); nose hace algo?
	return 0;
}
