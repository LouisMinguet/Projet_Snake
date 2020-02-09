# Makefile pour le Snake

# CHAPITRE 1 : BUT FINAL

but : startgame

# CHAPITRE 2 : VARIABLES

OFILES = jeu.o \
         option.o \
         launcher.o

CC = gcc

CFLAGS = -Wall -ansi -pedantic -g

SRC= jeu.c launcher.c option.c

OBJ= $(SRC:.c=.o)

LIBS = -lgraph

# CHAPITRE 3 : DEPENDANCES

startgame : jeu.o option.o launcher.o

jeu.o : jeu.h

option.o : option.h

launcher.o : jeu.h option.h

#CHAPITRE 4 : DEPENDANCES AVEC COMMANDES

jeu.o : jeu.c jeu.h
	$(CC) $(CFLAGS) -c jeu.c 

option.o : option.c option.h
	$(CC) $(CFLAGS) -c option.c
	
startgame : $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) $(LIBS) -o startgame 

#CHAPITRE 5 : NETTOYAGE DES FICHIERS GENERES

clean :
	-rm -f $(OFILES) startgame

#CHAPITRE 6 : BUTS FACTICES

.PHONY : but clean