#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXBUFFERLEN 1024
#define SERVERNAME "localhost"
#define SERVERPORT "12345"

int main(int argc, char* argv[]){
	int descSock;
	int ecode;
	struct addrinfo *res,*resPtr;
	struct addrinfo hints;
	char buffer[MAXBUFFERLEN];
	bool isConnected = false;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	
	ecode = getaddrinfo(SERVERNAME,SERVERPORT,&hints,&res);
	if (ecode){
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ecode));
		exit(1);
	}
	
	resPtr = res;
	
	while(!isConnected && resPtr!=NULL){
		descSock = socket(resPtr->ai_family, resPtr->ai_socktype, resPtr->ai_protocol);
		if (descSock == -1) {
			perror("Erreur creation socket");
			exit(2);
		}
		
		ecode = connect(descSock, resPtr->ai_addr, resPtr->ai_addrlen);
		if (ecode == -1) {
			resPtr = resPtr->ai_next;
			close(descSock);
		}
		else isConnected = true;
	}
	
	freeaddrinfo(res);
	if (!isConnected){
		perror("Connexion impossible");
		exit(2);
	}
	
	ecode = read(descSock, buffer, MAXBUFFERLEN-1);
	if (ecode == -1) {perror("Problème de lecture\n"); exit(3);}
	buffer[ecode] = '\0';
	printf("\"%s\".\n",buffer);
	close(descSock);
}
