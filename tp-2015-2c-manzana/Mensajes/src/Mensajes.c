/*
 ============================================================================
 Name        : Mensajes.c
 Author      : Nicolas
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(void) {
	/* CREA EL PUERTO ESCUCHA DEL SERVIDOR	*/
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(6001);

	int servidor = socket(AF_INET, SOCK_STREAM, 0);

	int acrivado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &acrivado, sizeof(acrivado));

	if (bind(servidor, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("fallo el bind");
		return 1;
	}
	printf ("esty escuchando n");
	listen(servidor, 100);

	/* servidor acepta un cliente */

	struct sockaddr_in direccionCliente;
	unsigned int len;
	int cliente = accept(servidor, (void*) &direccionCliente, &len);
	 printf("recibi una conexion en ");

	 /*recivir mensajes de cliente */

	 uint32_t tamanoMensaje;
	 tamanoMensaje = 1;
	 char* buffer = malloc(tamanoMensaje);
	 int bytesRecibidos = recv( cliente, buffer, tamanoMensaje, MSG_WAITALL);
	 if (bytesRecibidos <= 0){
		 perror("erreo en la conexion");
		 return 1;
	 }
	 free(buffer);

	return 0;
}
