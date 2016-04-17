/*
 ============================================================================
 Name        : AdministradorDeSwap.c
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


#define PUERTO_Escucha_Swap "6930"
#define BACKLOG 5
#define PACKAGESIZE 1024


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

    int socket_escucha_swap;
    if (crear_servidor( &socket_escucha_swap, PUERTO_Escucha_Swap) != 0){
        perror("error creando socket_escucha_swap");
        exit(1);
    }


	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	int socketCliente = accept(socket_escucha_swap, (struct sockaddr *) &addr, &addrlen);
    if (socketCliente < 0){
        perror("error aceptando socketCliente");
        exit(1);
    }
//-----------------------------------------------------------------------------------------
      // fin de el proseso de conexiones

	char package[PACKAGESIZE];
	int status = 1;

	printf("Administrador de Memoria conectado. Esperando mensajes:\n");

	while (status != 0){
		status = recv(socketCliente, (void*) package, PACKAGESIZE, 0);
		if (status != 0) printf("%s", package);

	}

	close(socketCliente);
	close(socket_escucha_swap);

	return 0;
}
