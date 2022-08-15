CC = gcc
CFLAGS  = -g -Wall
default: main

# Pour créer l'executable main, on a besoin de
# Identite.o, ABR.o:
main: 
	$(CC) $(CFLAGS) -o main main.c hashtable.c person.c

# Pour supprimer tous les objets 'make clean'
clean: 
	$(RM) main *.o *~