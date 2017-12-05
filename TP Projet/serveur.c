#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define SERVADDR "localhost"
#define SERVPORT "12345"
#define LISTENLEN 1
#define MAXBUFFERLEN 1024
#define MAXHOSTLEN 1024
#define MAXPORTLEN 1024

int main(){
	int ecode;
	char serverAddr[MAXHOSTLEN];
	char serverPort[MAXPORTLEN];
	int descSockRDV;
	int descSockCOM;
	
	struct addrinfo hints;
	struct addrinfo *res;
	struct sockaddr_storage myinfo;
	struct sockaddr_storage from;
	
	socklen_t len;
	char buffer[MAXBUFFERLEN];
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	
	ecode = getaddrinfo(SERVADDR, SERVPORT, &hints, &res);
	if (ecode) {
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ecode));
		exit(1);
	}
	
	descSockRDV = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (descSockRDV == -1) {
		perror("Erreur creation socket");
		exit(4);
	}
	
	ecode = bind(descSockRDV, res->ai_addr, res->ai_addrlen);
	if (ecode == -1) {
		perror("Erreur liaison de la socket de RDV");
		exit(3);
	}
	
	freeaddrinfo(res);
	len=sizeof(struct sockaddr_storage);
	ecode=getsockname(descSockRDV, (struct sockaddr *) &myinfo, &len);
	
	if (ecode == -1)
	{
		perror("SERVEUR: getsockname");
		exit(4);
	}
	
	ecode = getnameinfo((struct sockaddr*)&myinfo, sizeof(myinfo),
	serverAddr,MAXHOSTLEN,
	serverPort, MAXPORTLEN,
	NI_NUMERICHOST | NI_NUMERICSERV);
	if (ecode != 0) {
		fprintf(stderr, "Erreur dans getnameinfo: %s\n", gai_strerror(ecode));
		exit(4);
	}
	printf("L'adresse d'ecoute est: %s\n", serverAddr);
	printf("Le port d'ecoute est: %s\n", serverPort);
	
	ecode = listen(descSockRDV, LISTENLEN);
	if (ecode == -1) {
		perror("Erreur initialisation buffer d'Ã©coute");
		exit(5);
	}
	
	len = sizeof(struct sockaddr_storage);
	descSockCOM = accept(descSockRDV, (struct sockaddr *) &from, &len);
	if (descSockCOM == -1){
		perror("Erreur accept\n");
		exit(6);
	}
	
	strcpy(buffer, "BLABLABLA\n");
	ecode = write(descSockCOM, buffer, strlen(buffer));
	if (ecode == -1) {perror("Problème d'écriture\n"); exit(7);}
	close(descSockCOM);
	close(descSockRDV);
}
