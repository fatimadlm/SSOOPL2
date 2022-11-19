all: minishell //Por defecto se generara el objeto all
//Creamos las variables
CC = gcc
CFLAGS = -g -Wall
//Indicamos la estructura de dependencias
minishell: minishell.o entrada_minishell.o ejecutar.o libmemoria.o redirecciones.o libshell.a
	$(CC) $(CFLAGS) minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o internas.o parser.o -o minishell
	
minishell.o: minishell.c ejecutar.h entrada_minishell.h internas.h parser.h
	$(CC) $(CFLAGS) -c minishell.c	
	
ejecutar.o: ejecutar.c ejecutar.h libmemoria.h redirecciones.h parser.h
	$(CC) $(CFLAGS) -c ejecutar.c

entrada_minishell.o: entrada_minishell.h entrada_minishell.c
	$(CC) $(CFLAGS) -c entrada_minishell.c
	
libmemoria.o: libmemoria.h libmemoria.c
	$(CC) $(CFLAGS) -c libmemoria.c

redirecciones.o: redirecciones.c redirecciones.h
	$(CC) $(CFLAGS) -c redirecciones.c

libshell.a: parser.o internas.o
	ar crs libshell.a parser.o internas.o
	
//Limpiamos todos los *.o cuando realizamos el clean	
.PHONY: clean
clean:
	rm -f minishell.o
	rm -f entrada_minishell.o
	rm .f ejecutar.o
	rm -f libmemoria.o
	rm -f redirecciones.o
	rm -f libshell.a
	rm -f minishell
