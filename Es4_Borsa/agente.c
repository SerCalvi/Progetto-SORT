#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <unistd.h>
#include <netdb.h>


#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	ItemType i;
	int ricavo;
	if (argc < 5) { 
		printf("Usage: %s other data\n", argv[0]);
		exit(-1);
	}
	
	strcpy(i.nome, argv[1]);
	i.quant = atoi(argv[2]);
	i.prezz_T = atoi(argv[3]);
	i.prezz_min = atoi(argv[4]);
	
	
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	serv_addr.sin_port = htons(port);
	
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket\n");
		exit(1);
	}    

	if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}

	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, &i, sizeof(i), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("waiting response \n");
	
	if ( recv(sockfd, &ricavo, sizeof(ricavo), 0) == -1 ) 
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}

	printf("Hai guadagnato: %d\n", ricavo);

	close(sockfd);

	return 0;
}



