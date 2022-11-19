#Por defecto se genera el objeto all
all: minishell 
#Definicion de variables automaticas
CC = gcc
CFLAGS = -g -Wall
#Creacion minishell
minishell: minishell.o entrada_minishell.o ejecutar.o libmemoria.o redirecciones.o libshell.a
	$(CC) $(CFLAGS) minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o internas.o parser.o -o minishell
#Obtencion  de minishell.o
minishell.o: minishell.c ejecutar.h entrada_minishell.h internas.h parser.h
	$(CC) $(CFLAGS) -c minishell.c	
#Obtencion  de ejecutar.o
ejecutar.o: ejecutar.c ejecutar.h libmemoria.h redirecciones.h parser.h
	$(CC) $(CFLAGS) -c ejecutar.c
#Obtencion  de entrada_minishell.o
entrada_minishell.o: entrada_minishell.h entrada_minishell.c
	$(CC) $(CFLAGS) -c entrada_minishell.c
#Obtencion  de libmemoria.o	
libmemoria.o: libmemoria.h libmemoria.c
	$(CC) $(CFLAGS) -c libmemoria.c
#Obtencion  de redirecciones.o
redirecciones.o: redirecciones.c redirecciones.h
	$(CC) $(CFLAGS) -c redirecciones.c
#Obtencion  de libshell.a
libshell.a: parser.o internas.o
	ar crs libshell.a parser.o internas.o 
	#compilamos las bibliotecas
	
#Limpiamos todos los *.o cuando ejecutamos clean	
.PHONY: clean
clean:
	rm -f minishell.o
	rm -f entrada_minishell.o
	rm .f ejecutar.o
	rm -f libmemoria.o
	rm -f redirecciones.o
	rm -f libshell.a
	rm -f minishell
