all:	guida visitatori server 

guida:	guida.c list.c
	gcc -Wall -o guida guida.c list.c

visitatori:	visitatori.c list.c
	gcc -Wall -o visitatori visitatori.c list.c

server:	server.c list.c
	gcc -Wall -o server server.c list.c


clean:
	rm -f guida visitatori server *~

