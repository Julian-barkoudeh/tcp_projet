# Modèle de fichier Makefile, à adapter pour le TP

# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS = 
LIBSDIR = 

# fichiers du projetcl
SRC = server.c communication.c serverUDP.c
OBJ = $(SRC:.c=.o)
EXEC1 = tcp.out
EXEC1 = udp.out

# règle initiale
all: tcp.out udp.out

# dépendance des h
communication.o : communication.h
server.o : communication.h 
serverUDP.o : communication.h 

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<
	
# règles d'édition de liens
tcp.out: server.o communication.o
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)
udp.out: serverUDP.o communication.o
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)
# règles supplémentaires
clean:
	rm -f *.o
mproper:
	rm -f $(EXEC) *.o