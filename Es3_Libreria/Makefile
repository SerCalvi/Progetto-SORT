all:	client_C client_L server 

client_C:	client_C.c
	gcc -Wall -o client_C client_C.c

client_L:	client_L.c
	gcc -Wall -o client_L client_L.c

server:	server.c list.c
	gcc -Wall -o server server.c list.c


clean:
	rm -f client_C client_L server *~

