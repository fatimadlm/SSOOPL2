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
	waitpid(-1, &estado, WNOHANG);  // Waitpid espera a que cualquier hijo (significado del -1)
					// cambie de estado, y que vuelva inmediatamente si ningun hijo ha salido (significado del WNOHANG)
					// es decir, se espera a que cualquier hijo cambie (si acaba de cambiar o no existe se devuleve "immediately")
}

int main (int argc, char *argv[])
{ int pipefd[2];
  pipe(pipefd[2]); /* Creacion de la tuberia por el padre */
  switch(fork()){
	case -1:perror("Error al crear el primer hijo");
		exit(1);
	case 0: /* Primer hijo ejecuta ls y escribe en tuberia */			 close(pipefd[0]); /* Descriptor de lectura no usado */
		/* Duplicar entrada de descriptor de escritura de tuberia en la */
		/* de stdout y cierre de descriptor */
		if (pipefd[1] != STDOUT_FILENO) {
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);}
		execlp("ls", "ls", (char *) NULL); /* Escribe en tuberia */
		perror("El primer hijo fallo en exec");
		exit(1);
	default: /* El padre pasa a crear otro hijo */
	break;} 
  switch(fork()) {
	case -1:perror("Error al crear el segundo hijo");
	case 0: /* Segundo hijo ejecuta el filtro wc leyendo de la tuberia */
		close(pipefd[1]); /* Descriptor de escritura no usado */
				/* Duplicar entrada de descriptor de lectura de tuberia en la */
				/* de stdin y cierre de descriptor */
		if (pipefd[0] != STDIN_FILENO) {
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);}
		execlp("wc", "wc", (char *) NULL); /* Lee de tuberia */
		perror("El segundo hijo fallo en exec");
		exit(1);
	default:
	break;}

/* El padre cierra los descriptores no usados de la tuberia */
  close(pipefd[0]);
  close(pipefd[1]);
/* Y espera a los procesos hijos */
  for (i=0; i < 2; i++)
  wait(NULL);
  exit(EXIT_SUCCESS)  
	char buf [BUFSIZ];				// Variable donde se almacena la orden
	struct sigaction sa;				// define el sigaction en la posicion de memoria sa
	memset(&sa, 0, sizeof(sa));			// memset llena la direccion de memoria de sigaction de 0. (lo vacia)
	sa.sa_handler = manejar_sigchild;		// especifica el signum como nada y realiza el waitpid anterior
	sa.sa_flags = SA_NOCLDSTOP | SA_RESTART; 	// las flags cambian como funciona sigaction, estas flags hacen:
							// NOCLDSTOP -> no recibes avisos cauando los procesos del hijo paran o continuan.
							// RESTART -> puedes resetear algunas llamadas al sistema que estan a traves de signals.
	sigaction(SIGCHLD, &sa, NULL);  		// sigaction cambia la acción de un proceso cuando recibe una señal.
							// en este caso, la señal es SIGCHILD. Como act es no nulo y oldact es nulo,
							// SIGCHILD se instala en &sa y  sustituye al null
							// Es decir, cuando el hijo de un proceso termina, se guarda el estado en sa ?
	
	while(1)
	{
		
		imprimir_prompt();			// Imprime la pantalla incial de la minishell*/ 
		leer_linea_ordenes(buf);		// Lee el texto introducido desde la minishell y lo almacena en memoria 
						  	// añadiendo el carácter de fin de linea
		if (strcmp(buf,"exit")==0)		// Comprueba que la cadena introducida es "exit", en caso afirmativo se deja de ejecutar la minishell
		{				 
			break;
		}
		else
	   	{   	
	   		if (es_ord_interna(buf)!=1)     // Comprueba la naturaleza de la orden, si la función devuelve un 1
						        // es una orden interna
	   		{
	   			//printf("Ejecutar linea de ordenes ");
	   			ejecutar_linea_ordenes(buf);
	   		}
	   		else
	   		{
	   			//printf("Ejecutar orden interna");
	   			ejecutar_ord_interna(buf);
	   		}
	   	}
	}
	exit(EXIT_SUCCESS); 
	return 0;
}
