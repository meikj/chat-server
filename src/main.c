/*
 * main.c
 *
 * Chat server programmed in C. This is done with educational purposes as an
 * aid to learn both C and network programming (at the moment Windows network
 * programming). There is no license attached with this software.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>

#include "logger.h"
#include "socket.h"

/*
 * Program is run as follows:
 *		<prog_name> <host> <port_number>
 *
 * If arguments don't conform as such, then program will exit
 *
 */
int main(int argc, char *argv[]) {
	// Check if program conforms to argument specification
	if(argc != 3) {
		printf("Usage: %s <host> <port_number>\n", argv[0]);
		exit(1);
	}

	// Listen socket variables
	int l_socket;
	char *l_host;
	int l_port;
	struct sockaddr_in l_addr;

	// Client socket variables
	int c_socket;

	l_socket = init_socket();
	l_host = argv[1];
	l_port = atoi(argv[2]);

	// Do some primitive port checking
	if(l_port == 0) {
		log_entry(LOGGER_ERROR, 0, "main(): invalid port specified");
		cleanup(l_socket, 1);
		exit(1);
	}

	// Output address and port for debugging purposes
	log_entry(LOGGER_DEBUG, 0, "main(): l_host = %s", l_host);
	log_entry(LOGGER_DEBUG, 0, "main(): l_port = %d", l_port);

	// Create address structure and bind socket
	l_addr = init_server_addr(l_host, l_port);

	if((bind(l_socket, (struct sockaddr *)&l_addr, sizeof l_addr)) == SOCKET_ERROR) {
		log_entry(LOGGER_ERROR, WSAGetLastError(), "main(): bind() failed");
		cleanup(l_socket, 1);
		exit(1);
	}

	log_entry(LOGGER_DEBUG, 0, "main(): bind() was successful: address = %s, port = %i",
		l_host, l_port);

	if(listen(l_socket, MAX_BACKLOG) == SOCKET_ERROR) {
		log_entry(LOGGER_ERROR, WSAGetLastError(), "main(): listen() failed");
		cleanup(l_socket, 1);
		exit(1);
	}

	log_entry(LOGGER_DEBUG, 0, "main(): listen() was successful: socket = %d, backlog = %d",
		l_socket, MAX_BACKLOG);

	// Wait for connection
	log_entry(LOGGER_INFO, 0, "main(): waiting for connection(s)...");

	for(;;) {
		// Main program loop
		c_socket = accept(l_socket, NULL, NULL);

		if(c_socket == INVALID_SOCKET) {
			log_entry(LOGGER_ERROR, WSAGetLastError(), "main(): accept() failed");
			cleanup(l_socket, 1);
			exit(1);
		}

		log_entry(LOGGER_INFO, 0, "main(): accept() was successful: c_socket = %d", c_socket);
	}

	// Perform final clean up code
	cleanup(l_socket, 1);

	return 0;
}
