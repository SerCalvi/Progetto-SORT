#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include <unistd.h>
#include <netdb.h>
#include <time.h>

#define Gmin 5
#define BUF_SIZE 1000


int port = 8000;


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	
	LIST giocatori;
	giocatori = NewList();
	LIST storico;
	storico = NewList();
	// Socket opening
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket");
		exit(1);
	}
	
	int options = 1;
	if(setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof (options)) < 0) {
		perror("Error on setsockopt");
		exit(1);
	}

	bzero( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}


	socklen_t address_size = sizeof( cli_addr );	

	while(1) 
	{
		printf("\n---waiting for players---\n");
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		ItemType item;
		char nome[50];
		// Message reception
		if (recv(newsockfd, &nome, sizeof(nome), 0 ) == -1) {
			perror("Error on receive");
			exit(1);
		}

		strcpy(item.nome, nome);
		item.socked = newsockfd;
		if (ThereIsItem(storico, item)== 0){
			item.partite = 0;
			item.punti = 0;
			giocatori = EnqueueFirst(giocatori, item);
			storico = EnqueueFirst(storico, item);
		}
		else{
			LIST tmp;
			tmp = Find(storico, item);
			tmp->item.socked = newsockfd;
			giocatori = EnqueueFirst(giocatori, tmp->item);
		}

		printf("Il palmares dei giocatori:\n");
		PrintList(storico);
		printf("Lista giocatori in attesa:\n");
		PrintList(giocatori);

		int numG = getLength(giocatori);
		int G_mancanti = Gmin - numG;
		if (send(newsockfd, &G_mancanti, sizeof(G_mancanti), 0 ) == -1) {
			perror("Error on send");
			exit(1);
		}
		
		if(getLength(giocatori) >= Gmin){
			int i;
			for(i = 3; i > 0; i--){
				srand(time(NULL));
				int num = rand() % getLength(giocatori);
				int j;
				LIST tmp = giocatori;
				for(j = 0; j < num; j++){
					tmp = tmp->next;
				}
				LIST tmp_Item = Find(storico, tmp->item);
				tmp_Item->item.partite = tmp_Item->item.partite + 1;
				tmp_Item->item.punti = tmp_Item->item.punti + i;
				if (send(tmp_Item->item.socked, &i, sizeof(i), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}	
				close(tmp_Item->item.socked);
				giocatori = Dequeue(giocatori, tmp->item);

			}
			while (!isEmpty(giocatori)){
				LIST tmp_Item;
				tmp_Item = Find(storico, giocatori->item);
				tmp_Item->item.partite = tmp_Item->item.partite  + 1;
				if (send(giocatori->item.socked, &i, sizeof(i), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
				close(giocatori->item.socked);	
				giocatori = Dequeue(giocatori, giocatori->item);
				
			}			
			printf("Il palmares dei giocatori:\n");
			PrintList(storico);
		}
	
	}

	close(sockfd);
	return 0;
}



