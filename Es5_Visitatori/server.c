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
	LIST guide;
	LIST visitors;
	visitors = NewList();
	guide = NewList();

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
		item.socked = newsockfd;
		if(strcmp(item.nome, " ") != 0){
			printf("Guide %s ready for a tour\n", item.nome);
			guide = EnqueueOrdered(guide, item);
		}
		else{
			printf("%d visitors are ready for a tour\n", item.np);
			visitors = EnqueueLast(visitors, item);
			
		}
		PrintList(guide);
		LIST tmp = visitors;
		while(!isEmpty(tmp)){
			LIST tmp1 = guide;
			while(!isEmpty(tmp1)){
				if(tmp->item.np > tmp1->item.quant_min && tmp->item.np < tmp1->item.quant_max){
					//send number of visitors to guide 
					if(send(tmp1->item.socked, &tmp->item.np, sizeof(int), 0 ) == -1){
						perror("Erron in send to guide!!\n");
						exit(1);
					}
					//send name of guide to visitors
					if(send(tmp->item.socked, &tmp1->item, sizeof(tmp1->item), 0 ) == -1){
						perror("Erron in send to guide!!\n");
						exit(1);
					}
					close(tmp1->item.socked);
					close(tmp->item.socked);
					guide = Dequeue(guide, tmp1->item);
					visitors = Dequeue(visitors, tmp->item);
					break;
				}
				tmp1 = tmp1->next;
			}
			tmp = tmp->next;

		}
		
	}

	close(sockfd);
	return 0;
}



