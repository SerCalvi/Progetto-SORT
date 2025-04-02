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
	ItemType item;
	strcpy(item.nome, " ");
	char* agente = malloc(30*sizeof(char));
	int lenght, i, control;
	
	
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
	if ( send(sockfd, &item, sizeof(item), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	if ( recv(sockfd, &lenght, sizeof(lenght), 0) == -1 ) 
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}
	for(i = 0; i < lenght; i++){
		if ( recv(sockfd, &item, sizeof(item), 0) == -1 ) {
			perror("Error in receiving response from server\n");
			exit(1);
		}
		PrintItem(item);
	}
	
	printf("Con chi vuoi investire?\n");
	scanf("%s", agente);
	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, agente, sizeof(agente), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}
	if ( recv(sockfd, &control, sizeof(control), 0) == -1 ) {
		perror("Error in receiving response from server\n");
		exit(1);
	}
	if(control == 1){
		printf("Ottimo hai aquistato un titolo.\n");
	}
	else{
		printf("Agente non presente nella lista\n");
	}

	close(sockfd);

	return 0;
}



