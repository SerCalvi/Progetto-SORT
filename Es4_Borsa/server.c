#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include <unistd.h>
#include <netdb.h>


#define BUF_SIZE 1000


int port = 8000;


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	ItemType item;
	LIST agenti;
	agenti = NewList();

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
		printf("\n---waiting client---\n");
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		// Message reception
		if (recv(newsockfd, &item, sizeof(item), 0 ) == -1) {
			perror("Error on receive");
			exit(1);
		}
		if(strcmp(item.nome, " ") != 0){
			printf("Benvenuto agente %s\n", item.nome);
			item.ricavo = 0;
			item.socked = newsockfd;
			agenti = EnqueueFirst(agenti, item);
		}
		else{
			printf("Buongiorno investitore\n");
			int length = getLength(agenti);
			PrintList(agenti);
			if (send(newsockfd, &length, sizeof(length), 0 ) == -1) {
				perror("Error on send");
				exit(1);
			}
			LIST tmp = agenti;
			while(!isEmpty(tmp)){
				if (send(newsockfd, &tmp->item, sizeof(tmp->item), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
				tmp = tmp->next;
			}
			char* agent = malloc(30*sizeof(char));
			if (recv(newsockfd, agent, sizeof(agent), 0 ) == -1) {
				perror("Error on receive");
				exit(1);
			}
			ItemType a;
			strcpy(a.nome, agent);
			int control = ThereIsItem(agenti, a);
			if (send(newsockfd, &control, sizeof(control), 0 ) == -1) {
				perror("Error on send");
				exit(1);
			}
		
			close(newsockfd);
			
			if(control == 1){
				tmp = agenti;
				while(!isEmpty(tmp)){
					if(strcmp(agent, tmp->item.nome) == 0){
						tmp->item.ricavo = tmp->item.ricavo + tmp->item.prezz_T;
						tmp->item.prezz_T = tmp->item.prezz_T + 1;
						tmp->item.quant = tmp->item.quant - 1;
					}
					else{
						tmp->item.prezz_T = tmp->item.prezz_T - 1;
					}
					tmp = tmp->next;
				}
				tmp = agenti;
				while(!isEmpty(tmp)){
					if(tmp->item.quant == 0 || tmp->item.prezz_T < tmp->item.prezz_min){
						if (send(tmp->item.socked, &tmp->item.ricavo, sizeof(tmp->item.ricavo), 0 ) == -1) {
							perror("Error on send");
							exit(1);
						}
						close(tmp->item.socked);
						ItemType t = tmp->item;
						tmp = tmp->next;
						agenti = Dequeue(agenti, t);
					}
					else
						tmp = tmp->next;
				}
			}
		}
	}

	close(sockfd);
	return 0;
}



