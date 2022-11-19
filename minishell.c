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
	waitpid(-1, &estado, WNOHANG); // el waitpid espera a cualquier hijo (significado del -1)
	//cambie de estado, y que vuelva inmediatamente si ningun hijo a salido (significado del WNOHANG)
	//Es decir, se espera a que cualquier hijo cambie (si acaba de cambiar o no existe se devuleve "immediately")
}

int main (int argc, char *argv[])
{
	char buf [BUFSIZ];			/*Variable donde se almacena la orden*/
	struct sigaction sa;				//define el sigaction en la posicion de memoria sa
	memset(&sa, 0, sizeof(sa));			//memset llena la direccion de memoria de sigaction de 0. (lo vacia)
	sa.sa_handler = manejar_sigchild;		//especifica el signum como nada y realiza el waitpid anterior
	sa.sa_flags = SA_NOCLDSTOP | SA_RESTART; 	//las flags cambian como funciona sigaction, estas flags hacen:
							//NOCLDSTOP -> no recibes avisos cauando los procesos del hijo paran o continuan.
							//RESTART -> puedes resetear algunas llamadas al sistema que estan a traves de signals.
	sigaction(SIGCHLD, &sa, NULL);  		//sigaction cambia la acción de un proceso cuando recibe una señal.
							//en este caso, la señal es SIGCHILD. Como act es no nulo y oldact es nulo,
							//SIGCHILD se instala en &sa y  sustituye al null
	//Es decir, cuando el hijo de un proceso termina, se guarda el estado en sa
	
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
