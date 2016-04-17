/*
 ============================================================================
 Name        : CPU.c
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


#define IP_Planificador "127.0.0.1"
#define IP_Memoria "127.0.0.1"
#define PUERTO_Planificador "5820"
#define PUERTO_Memoria "4710"
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

int main(){

	int clientePlanificador;
	int clienteMemoria;

	if (crear_cliente(&clientePlanificador, IP_Planificador, PUERTO_Planificador) != 0){
        perror("error creando clientePlanificador");
        exit(1);
	}
	printf("Planificador Conectado.\n");

	if (crear_cliente(&clienteMemoria, IP_Memoria, PUERTO_Memoria) != 0){
        perror("error creando clienteMemoria");
        exit(1);
	}
	printf("Conectado al Administrador de Memoria.\n");

	printf("Bienvenido al sistema, esperando mensajes:\n");
//-----------------------------------------------------------------------------------------
      // fin de el proseso de conexiones

	int status = 1;
	char package[PACKAGESIZE];

	while(status){
			status = recv(clientePlanificador, (void*) package, PACKAGESIZE, 0);
			if (status) send(clienteMemoria, package, strlen(package) + 1, 0);
			if (status != 0) printf("%s", package);
	}

	close(clientePlanificador);
	close(clienteMemoria);

	return 0;
}
