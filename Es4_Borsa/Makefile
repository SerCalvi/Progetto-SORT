all:	agente investitore server 

agente:	agente.c list.c
	gcc -Wall -o agente agente.c list.c

investitore:	investitore.c list.c
	gcc -Wall -o investitore investitore.c list.c

server:	server.c list.c
	gcc -Wall -o server server.c list.c


clean:
	rm -f agente investitore server *~

