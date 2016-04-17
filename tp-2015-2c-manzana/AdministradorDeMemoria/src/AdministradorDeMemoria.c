/*
 ============================================================================
 Name        : AdministradorDeMemoria.c
 Author      : Nicolas
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


#define IP_Memoria "127.0.0.1"
#define IP_Swap "127.0.0.1"
#define PUERTO_Memoria "4710"
#define PUERTO_Swap "6930"
#define BACKLOG 5
#define PACKAGESIZE 1024

int crear_cliente(int* socketCliente,char* IP,char* PUERTO){

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(IP, PUERTO, &hints, &serverInfo);

	*socketCliente = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	connect(*socketCliente, serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo);
	return 0;
}

int crear_servidor(int* listenningSocket, char* PUERTO){
	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(NULL, PUERTO, &hints, &serverInfo);

	*listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	bind(*listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo);

	listen(*listenningSocket, BACKLOG);
	return 0;
}

int main(){

	int clienteSwap;
	int servidorCPU;

	if (crear_cliente(&clienteSwap, IP_Swap, PUERTO_Swap) != 0){
        perror("error creando clienteSwap");
        exit(1);
	}

	printf("Conectado al Administrador de Swap.\n");

	if (crear_servidor(&servidorCPU, PUERTO_Memoria) != 0){
        perror("error creando servidorCPU");
        exit(1);
        }
	printf("CPU conectandose...\n");

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	int clienteCPU = accept(servidorCPU, (struct sockaddr *) &addr, &addrlen);
	if (clienteCPU < 0){
        perror("error aceptando clienteCPU");
        exit(1);
	}
	printf("CPU conectado.\n");

	printf("Bienvenido al sistema, esperando mensajes:\n");
//-----------------------------------------------------------------------------------------
      // fin de el proseso de conexiones


	int status = 1;
	char package[PACKAGESIZE];

	while(status){
			status = recv(clienteCPU, (void*) package, PACKAGESIZE, 0);
			if (status) send(clienteSwap, package, strlen(package) + 1, 0);
			if (status != 0) printf("%s", package);
	}

	close(clienteSwap);
	close(clienteCPU);
	close(servidorCPU);

	return 0;
}
