/*
 ============================================================================
 Name        : Planificador.c
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


#define PUERTO_Escucha_Planificador "5820"
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

    int Socket_escucha;
	if (crear_servidor( &Socket_escucha , PUERTO_Escucha_Planificador) != 0){
        perror("error creando Socket_escucha");
        exit(1);
	}

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	int socketCliente = accept(Socket_escucha, (struct sockaddr *) &addr, &addrlen);
	if (socketCliente < 0){
        perror("error al aceptar socketCliente");
        exit(1);
        }
    printf("Conectado al CPU. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");
//-----------------------------------------------------------------------------------------
      // fin de el proseso de conexiones


	char package[PACKAGESIZE];
	int enviar = 1;



	while(enviar){
			fgets(package, PACKAGESIZE, stdin);
			if (!strcmp(package,"exit\n")) enviar = 0;
			if (enviar) send(socketCliente, package, strlen(package) + 1, 0);
	}

	close(socketCliente);
	close(Socket_escucha);

	return 0;
}
