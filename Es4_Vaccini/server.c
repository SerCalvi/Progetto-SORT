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
	LIST fornitori;
	LIST centri;
	fornitori = NewList();
	centri = NewList();
	LIST tmp1;
	int p;

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
	
   char buf[BUF_SIZE];	

	while(1) 
	{
		printf("\nWaiting for a new connection...\n");

		printf("\nLista centri in attesa: \n");
		PrintList(centri);
		printf("\nLista fornitori in attesa: \n");
		PrintList(fornitori);

		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		bzero(buf, BUF_SIZE);
		ItemType i;
		// Message reception
		if ( recv(newsockfd, &i, sizeof(i), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}
		i.sock = newsockfd;
		if(i.quant == 0){
			printf("\n Ricevuto nuovo centro...");
			centri = EnqueueOrderedC(centri, i);
		}
		else{
			printf("\n Ricevuto nuovo fornitore...");
			fornitori = EnqueueOrderedF(fornitori, i);
		}

		printf("\nLista centri in attesa: \n");
		PrintList(centri);
		printf("\nLista fornitori in attesa: \n");
		PrintList(fornitori);

		tmp1 = fornitori;
		while(!isEmpty(tmp1)){
			LIST tmp;
			LIST tmp2;
			tmp = NewList();
			tmp2 = centri;
			p = 0;
			while(!isEmpty(tmp2)){
				int pt = p+tmp2->item.pazienti;
				if(pt <= tmp1->item.quant){
					p = pt;
					tmp = EnqueueLast(tmp, tmp2->item);
				}
				tmp2 = tmp2->next;
			}
			if(p >= tmp1->item.quant_min){
				printf("\nmando a %s, che ha questa socked: %d\n", tmp1->item.nome, tmp1->item.sock);
				int num = getLength(tmp);
				if ( send( tmp1->item.sock, &num, sizeof(num), 0 ) == -1){
					perror("Error on send");
					exit(1);
				}
				LIST temp = tmp;
				while(!isEmpty(temp)){
					if ( send( tmp1->item.sock, &temp->item, sizeof(temp->item), 0 ) == -1){
						perror("Error on send");
						exit(1);
					}
					temp = temp->next;
				}
				
				close(tmp1->item.sock);
				while(!isEmpty(tmp)){
					printf("mando a %s, che ha questa socked: %d\n", tmp->item.nome, tmp->item.sock);
					if ( send( tmp->item.sock, &tmp1->item, sizeof(tmp->item), 0 ) == -1){
						perror("Error on send");
						exit(1);
					}
					close(tmp->item.sock);
					centri = Dequeue(centri, tmp->item);
					tmp = tmp->next;
				}
				fornitori = Dequeue(fornitori, tmp1->item);
				
				
			}
			
			tmp1 = tmp1->next;
			
		}
		
	}

	close(sockfd);
	return 0;
}



