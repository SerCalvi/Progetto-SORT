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
	if(argc < 2){
		perror("Error : Miss data\nIscrizione fallita.\n");
		exit(1);
	}

	ItemType guida;
	int np = atoi(argv[1]);
	
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}
	strcpy(i.nome, " ");
	i.np = np;

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
	if ( send(sockfd, &i, sizeof(i), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	if ( recv(sockfd, &guida, sizeof(guida), 0) == -1 )
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}
	
	printf("%s ha il piacere di guidarti all'interno di questo museo\n", guida.nome);
	
	
	close(sockfd);

	return 0;
}



