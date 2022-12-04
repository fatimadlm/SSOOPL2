#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */

void redirec_entrada(char **args, int indice_entrada, int *entrada)
	{
	int file;
	file = open(args[indice_entrada+1], O_RDONLY);
	if (file == -1 ){ //En caso de error al abrir el archivo
		printf("Error redirec entrada\n");
	}
	else{ 
		*entrada = file; //Sustituimos la entrada por el descriptor del archivo
		args[indice_entrada] = '\0'; //El argumento '<' los ponemos a 0 al igual que el archivo para evitar errores
		args[indice_entrada+1] = '\0';
	}
}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida)
	{
	int file2;
	file2 = open(args[indice_salida+1],O_WRONLY | O_CREAT | O_TRUNC , 0600);
	if (file2 == -1 ){  //En caso de error al abrir el archivo
		printf("Error redirec salida\n");	
	}
	else{ 
		*salida = file2; //Sustituimos la salida por el descriptor del archivo
		args[indice_salida] = '\0';  //El argumento '>' los ponemos a 0 al igual que el archivo para evitar errores
		args[indice_salida+1] = '\0';
	}
}
