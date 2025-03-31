all:	fornitori centri server 

fornitori:	fornitori.c list.c
	gcc -Wall -o fornitori fornitori.c list.c

centri:	centri.c list.c
	gcc -Wall -o centri centri.c list.c

server:	server.c list.c
	gcc -Wall -o server server.c list.c


clean:
	rm -f fornitori centri server *~

