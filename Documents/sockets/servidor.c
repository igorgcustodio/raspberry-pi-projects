// ------------- Inclusão de bibliotecas -------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

// ------------- Definições -------------
#define PORTA 12345                         // Porta de conexão na qual os clientes se conectarão
#define FilaConexao 10                      // Número de conexões em fila

void main()
{
    int SocketServidor;                     // Socket para escuta do servidor
    int SocketCliente;                      // Socket para comunicação com novo cliente
    struct sockaddr_in EnderecoServidor;    // Informações sobre endereço do servidor
    struct sockaddr_in EnderecoCliente;     // Informação sobre endereço do cliente
    int tamanho;
    int estado = 0;

    // Cria um ponto de comunicação
    SocketServidor = socket(AF_INET,        // Domínio: AF_INET - Protocolo IPv4
        SOCK_STREAM,                        // Tipo: SOCK_STREAM - via de mão dupla, confiável, sequencial e baseada em conexão
        0);                                 // Protocolo: Especificar protocolos não descritos

    if( SocketServidor == -1)
    {
        perror("Erro ao iniciar EndPoint");
        return;
    }

    EnderecoServidor.sin_family         = AF_INET;      // Domínio: Família de endereços IPv4
    EnderecoServidor.sin_port           = htons(PORTA); // Porta: Porta de comunicação
    EnderecoServidor.sin_addr.s_addr    = INADDR_ANY;   // Inclui o endereço IP da comunicação de entrada
    bzero(&(EnderecoServidor.sin_zero), 8);             // Limpa a estrutura

    // Vincula novo socket
    estado = bind(SocketServidor,                       // Descritor do Socket
        (struct sockaddr *)&EnderecoServidor,           // Endereço em formato sockaddr
        sizeof(struct sockaddr));                       // Tamanho da estrutura de endereço

    if(estado == -1)
    {
        perror("Erro ao iniciar binding");
        return;
    }

    estado = 0;
    // Iniciar processo de escuta por clientes 
    estado = listen(SocketServidor,                      // Descritor do Socket
        FilaConexao);                                    // Backlog: número de conexões a serem enfileiradas

    switch(estado)
    {
        case -1:
                perror("Houve um erro ao iniciar processo de escuta");
                return;
        case EADDRINUSE:
                perror("Outro socket está escutando na mesma porta");
                return;
        default:
                break;
    }

    while(1) {

        tamanho = sizeof(struct sockaddr_in);

        SocketCliente = accept(SocketServidor,          // Descritor do Socket
            (struct sockaddr *)&EnderecoCliente,        // Estrutura de endereço
            &tamanho);                                  // Tamanho da estrutura

        if(SocketCliente < 0)
        {
            perror("Erro em aceitar conexão");
            continue;
        }

        printf("Cliente Conectado => %s\n",inet_ntoa(EnderecoCliente.sin_addr));
        
        if (send(SocketCliente, "Seja bem vindo!\n", 16, 0) == -1)
        {
            perror("send");
            close(SocketCliente);
            exit(0);
        }
        
        close(SocketCliente);
        
    }
}
