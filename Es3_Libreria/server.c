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

	
	LIST myList;
	myList = NewList();
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
		printf("\n---\npid %d: waiting connections\n", getpid());
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
			
		ItemType i;
		printf("pid %d child of %d: serving active connection\n", getpid(), getppid());
		// Message reception
		if (recv(newsockfd, &i, sizeof(i), 0 ) == -1) {
			perror("Error on receive");
			exit(1);
		}

		printf("pid %d: received Book Title:%s Number of Copies: %d; respond incrementing by 1 \n", getpid(), i.titolo, i.copie);
		
		if(i.copie > 0){
			int r;
			if(ThereIsItem(myList, i) == 0){
				myList = EnqueueFirst(myList, i);
				r = 0;
				if (send(newsockfd, &r, sizeof(r), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
			}
			else{
				r = 1;
				if (send(newsockfd, &r, sizeof(r), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
			}
		}
		else if(i.copie == 0){
			LIST tmp = Find(myList, i);
			int r;
			if(!isEmpty(tmp)){
				r = 0;
				if (send(newsockfd, &r, sizeof(r), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
				tmp->item.copie--;
				if(tmp->item.copie == 0)
					Dequeue(myList, tmp->item);
			}
			else{
				r = 1;
				if (send(newsockfd, &r, sizeof(r), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
			}
		}
		else{
			int r = -1;
				if (send(newsockfd, &r, sizeof(r), 0 ) == -1) {
					perror("Error on send");
					exit(1);
				}
		}
		

		
		close(newsockfd);
	}

	close(sockfd);
	return 0;
}



