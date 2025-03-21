#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>


#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	char stringa[50];
	int num, answer;
	
	if (argc < 2) { 
		printf("Usage: %s integer\n", argv[0]);
		exit(-1);
	}
	else if (argc < 3){
		strcpy(stringa,argv[1]);
		num = 1;

	}
	else{
		strcpy(stringa,argv[1]);
		num = atoi(argv[2]);
	}
	typedef struct {
		int copie;
		char titolo[50];
	} ItemType;
	
	ItemType L;
	L.copie = num;
	strcpy(L.titolo, stringa);

	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
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


	printf("Send the number %d\n", num);

	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, &L, sizeof(L), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("waiting response \n");
	
	if ( recv(sockfd, &answer, sizeof(answer), 0) == -1 ) 
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}
	if(answer == 0)
		printf("Libro distribuito con successo!\n");
	else if (answer == 1){
		printf("Azione invalida, libro presente nella lista.\n");
	}
	else
		printf("Errore inserimento dati");

	close(sockfd);

	return 0;
}



