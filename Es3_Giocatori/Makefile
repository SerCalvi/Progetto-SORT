all:	giocatori server 

giocatori:	giocatori.c
	gcc -Wall -o giocatori giocatori.c

server:	server.c list.c
	gcc -Wall -o server server.c list.c


clean:
	rm -f giocatori server *~

