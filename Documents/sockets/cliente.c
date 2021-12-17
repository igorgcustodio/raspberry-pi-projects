// ------------- Inclusão de bibliotecas -------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

// ------------- Definições -------------
#define PORTA 12345    				// Porta de conexão na qual os clientes se conectarão
#define MAXDATASIZE 100 			// Tamanho máximo da recepção

void main()
{
	int ClienteSocket;				// Socket para comunicação com servidor
	int numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in EnderecoServidor;
	int estado = 0;

	he = gethostbyname("127.0.0.1");   // Endereço do servidor
	if(he < 0)
	{
		herror("Erro ao converter Endereço do Servidor");
		exit(1);
	}

	ClienteSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(ClienteSocket == -1) 
	{
		perror("Erro ao inicar EndPoint");
		exit(1);
	}

	EnderecoServidor.sin_family 		= AF_INET;
	EnderecoServidor.sin_port 		= htons(PORTA);
	EnderecoServidor.sin_addr 		= *((struct in_addr *)he->h_addr);
	bzero(&(EnderecoServidor.sin_zero), 8);

	estado = connect(ClienteSocket,
		(struct sockaddr *)&EnderecoServidor, 
		sizeof(struct sockaddr));

	switch(estado)
	{
		case -1:
				perror("Erro ao conectar");
				return;
		case EADDRINUSE:
				perror("O Endereço já está em uso");
				return;
		default:
				break;
	}

	numbytes = recv(ClienteSocket, 
		buf, 
		MAXDATASIZE, 
		0);
	if(numbytes < 0)
	{
		perror("Problemas ao receber");
		exit(1);
	}

	buf[numbytes] = '\0';
	printf("Recebido: %s",buf);
	close(ClienteSocket);
	
	return;
}
